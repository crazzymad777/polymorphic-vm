#include <stdint.h>
#include <limits.h>
#include <stdio.h>
#include <povm.h>

typedef struct result (*datum_operation)(struct datum, struct datum);

int povm_execute_command(struct povm_state* vm, FILE* fd, union udatum* stack, int32_t* types);

int povm_execute(FILE* fd, union udatum* stack, int32_t* types) {
    struct povm_state vm = {
        fd,
        stack,
        fd,
        stack,
        types
    };

    while (!feof(fd)) {
		int r = povm_execute_command(&vm, fd, vm.stack, vm.types);
		if (r == -42) {
			return -42;
		}

		if (r != 0) {
			return r;
		}
	}
    return 0;
}

int povm_execute_command(struct povm_state* vm, FILE* fd, union udatum* stack, int32_t* types) {
    void apply_operation(datum_operation op) {
        struct result r = op(povm_datum(*(types-1), *(stack-1)), povm_datum(*(types), *(stack)));
        if (r.type == RESULT) {
            stack -= 1;
            types -= 1;
            *types = r.datum.type;
            stack->i64 = r.datum.i64;
        } else {
            fprintf(stderr, "Polymorhpic VM halt!\n");
            fprintf(stderr, "Error! Datum error: 0x%x\n", r.error_code);
            fprintf(stderr, "Offset: 0x%x\n", ftell(fd));
            // How to behave properly?
        }
    }


    int c = fgetc(fd);
    int64_t* sp = &stack->i64;

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
                    stack += 1;
                    types += 1;

                    stack->i64 = value;
                    *types = type;
                }
            }
        } else if (c == COMMAND_DROP) {
            // For CDT: must check internal counter of uses
            stack -= 1;
            types -= 1;
        } else if (c == COMMAND_SWAP) {
            int type = *(types-1);
            int64_t value = *(sp-1);
            *(types - 1) = *types;
            *(sp - 1) = stack->i64;
            *types = type;
            stack->i64 = value;
        } else if (c == COMMAND_DUP) {
            // For CDT: must increment internal counter of uses
            stack += 1;
            types += 1;
            *types = *(types - 1);
            *stack = *(stack - 1);
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
			int exit_code = povm_execute_command(&vm_callee, fd, stack, types);
			if (exit_code == -42) {
				exit_code = 0;
			}
			fseek(fd, pos + 9, SEEK_SET);
			*vm = vm_callee;
			return exit_code;
		} else if (c == COMMAND_RET) {
            vm->stack = stack;
            vm->types = types;
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
            fprintf(stderr, "Polymorhpic VM halt!\n");
            fprintf(stderr, "Error! Not implemented opcode: 0x%x\n", c);
            fprintf(stderr, "Offset: 0x%x\n", ftell(fd));
            return ERROR_UNIMPLEMENTED_OPCODE;
            /* struct datum p0 = povm_datum(*types, *stack);
            struct datum p1 = povm_datum(*(types-1), *(stack-1));

			uint64_t value = 0;
			// char equal = *(vm.sp_f64-1) == *vm.sp_f64;
			char greater = datum_op_gt(p0, p1);
			uint64_t less = datum_op_lt(p0, p1);
			char not_equal = datum_op_not_equals(p0, p1);

			value = not_equal | (greater << 1) | (less << 63);
			// Equal - 0
			// Not Equal - 1
			// Greater - 3
			// Less - -9223372036854775807

			// NaN != NaN -> 1
			// NaN != x -> 1
			stack += 1;
            types += 1;
			stack->i64 = value;
            *types = I64; */
		} else if (c == COMMAND_JE) {
			long pos = ftell(fd) - 1;
			uint64_t offset;
			int bytes = fread(&offset, 8, 1, fd);
			if (bytes == 1) {
				if (stack->i64 == 0 || (*types == BOOLEAN && stack->boolean)) {
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
				if (stack->i64 != 0 || (*types == BOOLEAN && stack->boolean)) {
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
				if (stack->i64 > 0 || (*types == BOOLEAN && stack->boolean)) {
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
				if (stack->i64 < 0 || (*types == BOOLEAN && stack->boolean)) {
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
				if (stack->i64 <= 0 || (*types == BOOLEAN && stack->boolean)) {
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
				if (stack->i64 >= 0 || (*types == BOOLEAN && stack->boolean)) {
					if (offset == 0) {
						fprintf(stderr, "Polymorhpic VM halt!\n");
						return -6;
					}
					fseek(fd, pos + offset, SEEK_SET);
				}
			}
		} else if (c == COMMAND_PRINT) {
            char buffer[32];
            datum_to_string(povm_datum(*types, *stack), buffer, 32);
            printf("%s", buffer);
        } else if (c == COMMAND_DEBUG_PRINT) {
            fprintf(stderr, "Offset: 0x%x\n", ftell(fd));
            fprintf(stderr, "Base stack pointer: 0x%x\n", vm->base_pointer);
            fprintf(stderr, "Stack pointer: 0x%x\n", vm->stack);
            char buffer[32];
            datum_to_string(povm_datum(*types, *stack), buffer, 32);
            printf("%s:%s\n", datum_get_type(*types), buffer);
        } else if (c == COMMAND_DEBUG_ASSERT) {
            // not strict equals, doubtfully
            bool assertion = datum_op_equals(povm_datum(*types, *stack), povm_datum(*(types-1), *(stack-1)));
            if (!assertion) {
                fprintf(stderr, "Polymorhpic VM: assertion failed\n");
                return -10;
            }
        } else if (c == COMMAND_EQUALS) {
            bool eq = datum_op_equals(povm_datum(*types, *stack), povm_datum(*(types-1), *(stack-1)));
            stack += 1;
            types += 1;
			stack->boolean = eq;
            *types = BOOLEAN;
        } else if (c == COMMAND_NOT_EQUALS) {
            bool not_eq = datum_op_not_equals(povm_datum(*types, *stack), povm_datum(*(types-1), *(stack-1)));
            stack += 1;
            types += 1;
			stack->boolean = not_eq;
            *types = BOOLEAN;
        } else if (c == COMMAND_GREAT_THAN) {
            bool gt = datum_op_gt(povm_datum(*types, *stack), povm_datum(*(types-1), *(stack-1)));
            stack += 1;
            types += 1;
			stack->boolean = gt;
            *types = BOOLEAN;
        } else if (c == COMMAND_LESS_THAN) {
            bool lt = datum_op_lt(povm_datum(*types, *stack), povm_datum(*(types-1), *(stack-1)));
            stack += 1;
            types += 1;
			stack->boolean = lt;
            *types = BOOLEAN;
        } else {
			fprintf(stderr, "Polymorhpic VM halt!\n");
			fprintf(stderr, "Error! Unknown opcode: 0x%x\n", c);
            fprintf(stderr, "Offset: 0x%x\n", ftell(fd));
			return ERROR_UNKNOWN_OPCODE;
		}
    }
    vm->stack = stack;
    vm->types = types;
    return 0;
}
