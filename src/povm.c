#include <stdint.h>
#include <limits.h>
#include <stdio.h>
#include <povm.h>


void udatum_to_string(enum datum_type type, union udatum datum, char* buffer, int siz);
static size_t codepoint_to_utf8(unsigned char *const buffer, const char32_t code);
int povm_execute_command(FILE* fd, union udatum* stack, int32_t* types);


FILE* base_fd;
void* base_pointer;

int povm_execute(FILE* fd, union udatum* stack, int32_t* types) {
    base_fd = fd;
    base_pointer = stack;
    while (!feof(fd)) {
		int r = povm_execute_command(fd, stack, types);
		if (r == -42) {
			return -42;
		}

		if (r != 0) {
			return r;
		}
	}
    return 0;
}

int povm_execute_command(FILE* fd, union udatum* stack, int32_t* types) {
    int c = fgetc(fd);
    int64_t* sp = &stack->i64;
	if (c != EOF) {
        if (c == COMMAND_PUSH_ROOT_STACK) {
            fprintf(stderr, "Polymorhpic VM halt!\n");
            fprintf(stderr, "Error! Not implemented opcode: 0x%x\n", c);
            return ERROR_UNIMPLEMENTED_OPCODE;
        } else if (c == COMMAND_PUSH) {
            int type;
            int64_t value;
            int bytes = fread(&type, 4, 1, fd);
            if (bytes == 4) {
                int bytes = fread(&value, 8, 1, fd);
                if (bytes == 8) {
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
        } else if (c == COMMAND_PRINT) {
            char buffer[32];
            udatum_to_string(*types, *stack, buffer, 32);
            printf("%s", buffer);
        } else {
			fprintf(stderr, "Polymorhpic VM halt!\n");
			fprintf(stderr, "Error! Unknown opcode: 0x%x\n", c);
			return ERROR_UNKNOWN_OPCODE;
		}
    }
    return 0;
}

void udatum_to_string(enum datum_type type, union udatum datum, char* buffer, int siz) {
    if (type == VOID) {
        snprintf(buffer, siz, "");
    }
    else if (type == BOOLEAN) {
        snprintf(buffer, siz, "%s", datum.boolean ? "true" : "false");
    }
    else if (type == I8) {snprintf(buffer, siz, "%d", datum.i8); return;}
    else if (type == I16) {snprintf(buffer, siz, "%d", datum.i16); return;}
    else if (type == I32) {snprintf(buffer, siz, "%d", datum.i32); return;}
    else if (type == I64) {snprintf(buffer, siz, "%d", datum.i64); return;}
    else if (type == U8) {snprintf(buffer, siz, "%d", datum.u8); return;}
    else if (type == U16) {snprintf(buffer, siz, "%d", datum.u16); return;}
    else if (type == U32) {snprintf(buffer, siz, "%d", datum.u32); return;}
    else if (type == U64) {snprintf(buffer, siz, "%d", datum.u64); return;}
    else if (type == F32) {snprintf(buffer, siz, "%f", datum.f32); return;}
    else if (type == F64) {snprintf(buffer, siz, "%f", datum.f64); return;}
    else if (type == CODEPOINT) {
        char buf[MB_LEN_MAX + 1] = {0};
		int sz = codepoint_to_utf8(buf, datum.codepoint);
		buf[sz] = '\0';
        snprintf(buffer, siz, "%s", buf);
    } else {
        snprintf(buffer, siz, "");
    }
}

__attribute__((visibility ("hidden")))
size_t codepoint_to_utf8(unsigned char *const buffer, const char32_t code)
{
    if (code <= 0x7F) {
        buffer[0] = code;
        return 1;
    }
    if (code <= 0x7FF) {
        buffer[0] = 0xC0 | (code >> 6);            /* 110xxxxx */
        buffer[1] = 0x80 | (code & 0x3F);          /* 10xxxxxx */
        return 2;
    }
    if (code <= 0xFFFF) {
        buffer[0] = 0xE0 | (code >> 12);           /* 1110xxxx */
        buffer[1] = 0x80 | ((code >> 6) & 0x3F);   /* 10xxxxxx */
        buffer[2] = 0x80 | (code & 0x3F);          /* 10xxxxxx */
        return 3;
    }
    if (code <= 0x10FFFF) {
        buffer[0] = 0xF0 | (code >> 18);           /* 11110xxx */
        buffer[1] = 0x80 | ((code >> 12) & 0x3F);  /* 10xxxxxx */
        buffer[2] = 0x80 | ((code >> 6) & 0x3F);   /* 10xxxxxx */
        buffer[3] = 0x80 | (code & 0x3F);          /* 10xxxxxx */
        return 4;
    }
    return 0;
}
