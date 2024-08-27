#include <povm-info.h>

enum povm_operand_type povm_info_get_operand_type(char opcode) {
    if (opcode == COMMAND_PUSH) {
        return DATUM;
    }
    if (opcode == COMMAND_CALL) {
        return OFFSET;
    }
    if (opcode == COMMAND_JMP) {
        return OFFSET;
    }
    if (opcode == COMMAND_FEED) {
        return TYPE;
    }
    return NO_OPERANDS;
}

