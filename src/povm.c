#include <povm-stack.h>
#include <stdint.h>
#include <limits.h>
#include <stdio.h>
#include <povm.h>

typedef struct result (*datum_operation)(struct datum, struct datum);

int povm_execute_command(struct povm_state* vm, FILE* fd, void* s);

int povm_execute(FILE* fd, union udatum* stack, int32_t* types) {
    struct povm_state vm = {
        fd,
        stack,
        fd,
        stack,
        types
    };

    while (!feof(fd)) {
		struct povm_stack s = {povm_get_std_stack_interface(), vm.types, vm.stack};
		int r = povm_execute_command(&vm, fd, &s);
		if (r == -42) {
			return -42;
		}

		if (r != 0) {
			return r;
		}
	}
    return 0;
}

int povm_execute_command(struct povm_state* vm, FILE* fd, void* s) {
	struct povm_stack* ctx = s;
	struct povm_stack_interface i = *ctx->interface;

    void apply_operation(datum_operation op) {
		struct datum d1 = i.get_datum(s);
		i.skip(s, -1);
		struct datum d0 = i.get_datum(s);
        struct result r = op(d0, d1);
        if (r.type == RESULT) {
            //i.skip(s, -1);
			i.put_datum(s, r.datum);
        } else {
            fprintf(stderr, "Polymorhpic VM halt!\n");
            fprintf(stderr, "Error! Datum error: %d\n", r.error_code);
            fprintf(stderr, "Offset: 0x%x\n", ftell(fd));
            // How to behave properly?
        }
    }


    int c = fgetc(fd);

	if (c != EOF) {
        if (c == COMMAND_PUSH_ROOT_STACK) {
            fprintf(stderr, "Polymorhpic VM halt!\n");
            fprintf(stderr, "Error! Not implemented opcode: 0x%x\n", c);
            fprintf(stderr, "Offset: 0x%x\n", ftell(fd));
            return ERROR_UNIMPLEMENTED_OPCODE;
        } else if (c == COMMAND_PUSH) {
            int type;
            int64_t value;
            int bytes = fread(&type, 4, 1, fd);
            if (bytes == 1) {
                int bytes = fread(&value, 8, 1, fd);
                if (bytes == 1) {
					i.skip(s, 1);
					i.put_datum(ctx, povm_datum_by_i64(type, value));
				}
            }
        } else if (c == COMMAND_DROP) {
            // For CDT: must check internal counter of uses
            i.skip(s, -1);
        } else if (c == COMMAND_SWAP) {
            struct datum d[2] = {i.get_datum(s)};

			i.skip(s, -1);
			d[1] = i.get_datum(s);
			i.put_datum(s, d[0]);

			i.skip(s, 1);
			i.put_datum(s, d[1]);
        } else if (c == COMMAND_DUP) {
            // For CDT: must increment internal counter of uses
            struct datum d = i.get_datum(s);
			i.skip(s, 1);
            i.put_datum(s, d);
        } else if (c == COMMAND_ADD) {
            apply_operation(datum_add);
        } else if (c == COMMAND_SUB) {
            apply_operation(datum_sub);
        } else if (c == COMMAND_MUL) {
            apply_operation(datum_mul);
        } else if (c == COMMAND_DIV) {
            apply_operation(datum_div);
        } else if (c == COMMAND_REM) {
            apply_operation(datum_rem);
        } else if (c == COMMAND_BXOR) {
            apply_operation(datum_bitwise_xor);
        } else if (c == COMMAND_BAND) {
            apply_operation(datum_bitwise_and);
        } else if (c == COMMAND_BOR) {
            apply_operation(datum_bitwise_or);
        } else if (c == COMMAND_CALL) {
			long pos = ftell(fd) - 1;
			uint64_t offset;
			int bytes = fread(&offset, 8, 1, fd);
			if (bytes == 1) {
				if (offset == 0) {
					fprintf(stderr, "Polymorhpic VM halt!\n");
					return -6;
				}
				fseek(fd, pos + offset, SEEK_SET);
			}

			struct povm_state vm_callee = *vm;

			int exit_code = 0;
			while (!feof(fd)) {;
				exit_code = povm_execute_command(&vm_callee, fd, &s);
				if (exit_code == -42) {
					break;
				}
			}

			if (exit_code == -42) {
				exit_code = 0;
			}
			fseek(fd, pos + 9, SEEK_SET);
			*vm = vm_callee;
			return exit_code;
		} else if (c == COMMAND_RET) {
            vm->stack = ctx->stack;
            vm->types = ctx->types;
			return -42;
		} else if (c == COMMAND_JMP) {
			long pos = ftell(fd) - 1;
			uint64_t offset;
			int bytes = fread(&offset, 8, 1, fd);
			if (bytes == 1) {
				if (offset == 0) {
					fprintf(stderr, "Polymorhpic VM halt!\n");
					return -6;
				}
				fseek(fd, pos + offset, SEEK_SET);
			}
		} else if (c == COMMAND_CMP) {
            struct datum p0 = i.get_datum(s);
			i.skip(s, -1);
            struct datum p1 = i.get_datum(s);

			uint64_t value = 0;
			char greater = datum_op_gt(p0, p1);
			uint64_t less = datum_op_lt(p0, p1);
			char not_equal = datum_op_not_equals(p0, p1);

			value = not_equal | (greater << 1) | (less << 31) | (less << 63);

			// NaN != NaN -> 1
			// NaN != x -> 1
			i.skip(s, 2);
			i.put_datum(s, povm_datum_by_i64(COMPARE_RESULT, value));
		} else if (c == COMMAND_JE) {
			long pos = ftell(fd) - 1;
			uint64_t offset;
			int bytes = fread(&offset, 8, 1, fd);
			if (bytes == 1) {
				struct datum datum = i.get_datum(s);
				if ((datum.type != BOOLEAN && datum.i64 == 0) || (datum.type == BOOLEAN && datum.boolean)) {
					if (offset == 0) {
						fprintf(stderr, "Polymorhpic VM halt!\n");
						return -6;
					}
					fseek(fd, pos + offset, SEEK_SET);
				}
			}
		} else if (c == COMMAND_JNE) {
			long pos = ftell(fd) - 1;
			uint64_t offset;
			int bytes = fread(&offset, 8, 1, fd);
			if (bytes == 1) {
				struct datum datum = i.get_datum(s);
				if ((datum.type && datum.i64 != 0) || (datum.type == BOOLEAN && !datum.boolean)) {
					if (offset == 0) {
						fprintf(stderr, "Polymorhpic VM halt!\n");
						return -6;
					}
					fseek(fd, pos + offset, SEEK_SET);
				}
			}
		} else if (c == COMMAND_JG) {
			long pos = ftell(fd) - 1;
			uint64_t offset;
			int bytes = fread(&offset, 8, 1, fd);
			if (bytes == 1) {
				if (i.get_value(s).i64 > 0) {
					if (offset == 0) {
						fprintf(stderr, "Polymorhpic VM halt!\n");
						return -6;
					}
					fseek(fd, pos + offset, SEEK_SET);
				}
			}
		} else if (c == COMMAND_JL) {
			long pos = ftell(fd) - 1;
			uint64_t offset;
			int bytes = fread(&offset, 8, 1, fd);
			if (bytes == 1) {
				if (i.get_value(s).i64 < 0) {
					if (offset == 0) {
						fprintf(stderr, "Polymorhpic VM halt!\n");
						return -6;
					}
					fseek(fd, pos + offset, SEEK_SET);
				}
			}
		} else if (c == COMMAND_JLE) {
			long pos = ftell(fd) - 1;
			uint64_t offset;
			int bytes = fread(&offset, 8, 1, fd);
			if (bytes == 1) {
				if (i.get_value(s).i64 <= 0) {
					if (offset == 0) {
						fprintf(stderr, "Polymorhpic VM halt!\n");
						return -6;
					}
					fseek(fd, pos + offset, SEEK_SET);
				}
			}
		} else if (c == COMMAND_JGE) {
			long pos = ftell(fd) - 1;
			uint64_t offset;
			int bytes = fread(&offset, 8, 1, fd);
			if (bytes == 1) {
				if (i.get_value(s).i64 >= 0) {
					if (offset == 0) {
						fprintf(stderr, "Polymorhpic VM halt!\n");
						return -6;
					}
					fseek(fd, pos + offset, SEEK_SET);
				}
			}
		} else if (c == COMMAND_PRINT) {
            char buffer[32];
            datum_to_string(i.get_datum(s), buffer, 32);
            printf("%s", buffer);
        } else if (c == COMMAND_DEBUG_PRINT) {
            fprintf(stderr, "Offset: 0x%x\n", ftell(fd));
            fprintf(stderr, "Base stack pointer: 0x%x\n", vm->base_pointer);
            fprintf(stderr, "Stack pointer: 0x%x\n", vm->stack);
            char buffer[32];
            datum_to_string(i.get_datum(s), buffer, 32);
            printf("%s:%s\n", datum_get_type(i.get_type(s)), buffer);
        } else if (c == COMMAND_DEBUG_ASSERT) {
            // not strict equals, doubtfully
			struct datum d0 = i.get_datum(s);
			i.skip(s, -1);
			struct datum d1 = i.get_datum(s);
			i.skip(s, 1);
            bool assertion = datum_op_equals(d0, d1);
            if (!assertion) {
                fprintf(stderr, "Polymorhpic VM: assertion failed\n");
                return -10;
            }
        } else if (c == COMMAND_EQUALS) {
            struct datum d0 = i.get_datum(s);
			i.skip(s, -1);
			struct datum d1 = i.get_datum(s);
			bool eq = datum_op_equals(d0, d1);
            i.skip(s, 2);
			i.put_datum(s, povm_datum_by_i64(COMPARE_RESULT, eq ? COMPARE_RESULT_EQUALS : COMPARE_RESULT_NOT_EQUALS));
			//stack->compare_result = eq ? COMPARE_RESULT_EQUALS : COMPARE_RESULT_NOT_EQUALS;
            //*types = COMPARE_RESULT;
        } else if (c == COMMAND_NOT_EQUALS) {
			struct datum d0 = i.get_datum(s);
			i.skip(s, -1);
			struct datum d1 = i.get_datum(s);
            bool not_eq = datum_op_not_equals(d1, d0);
            i.skip(s, 2);

			i.put_datum(s, povm_datum_by_i64(COMPARE_RESULT, not_eq ? COMPARE_RESULT_NOT_EQUALS : COMPARE_RESULT_EQUALS));
        } else if (c == COMMAND_GREAT_THAN) {
            struct datum d0 = i.get_datum(s);
			i.skip(s, -1);
			struct datum d1 = i.get_datum(s);
			bool gt = datum_op_gt(d0, d1);
			bool eq = datum_op_equals(d0, d1);
            i.skip(s, 2);

			int64_t result = eq ? COMPARE_RESULT_EQUALS : COMPARE_RESULT_NOT_EQUALS;
            if (gt) {
				result |= COMPARE_RESULT_GREATER;
			}
			i.put_datum	(s, povm_datum_by_i64(COMPARE_RESULT, result));
        } else if (c == COMMAND_LESS_THAN) {
            struct datum d0 = i.get_datum(s);
			i.skip(s, -1);
			struct datum d1 = i.get_datum(s);
			bool lt = datum_op_lt(d0, d1);
            bool eq = datum_op_equals(d0, d1);
            i.skip(s, 2);

			int64_t result = eq ? COMPARE_RESULT_EQUALS : COMPARE_RESULT_NOT_EQUALS;
            if (lt) {
				result |= COMPARE_RESULT_LESS;
			}
			i.put_datum(s, povm_datum_by_i64(COMPARE_RESULT, result));
        } else {
			fprintf(stderr, "Polymorhpic VM halt!\n");
			fprintf(stderr, "Error! Unknown opcode: 0x%x\n", c);
            fprintf(stderr, "Offset: 0x%x\n", ftell(fd));
			return ERROR_UNKNOWN_OPCODE;
		}
    }

	vm->stack = ctx->stack;
	vm->types = ctx->types;
	return 0;
}
