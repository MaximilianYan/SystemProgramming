/**
 * @file asmLangDSLInstructions.cpp
 * @author Maximilian (https://github.com/MaximilianYan)
 * @brief Instructions for mAsm language behaviour. File need to be included wherever you want to use command instructions.
 * @date 2022-10-17
 *
 * @copyright Copyright (c) 2022
 *
 */

DESCRIPT_COMMAND("hlt", 0x00, {
    return Error::OK_HALT;
    }
)
DESCRIPT_COMMAND("push", 0x01,
    {
    stack::push(&mainCPU->stack, commandArgs.argSum);
    }
)
DESCRIPT_COMMAND("add", 0x02, {
    AsmCode_t a = 0;
    AsmCode_t b = 0;
    stack::pop(&mainCPU->stack, &b);
    stack::pop(&mainCPU->stack, &a);
    stack::push(&mainCPU->stack, (AsmCode_t)(a + b));
    }
)
DESCRIPT_COMMAND("div", 0x03, {
    AsmCode_t a = 0;
    AsmCode_t b = 0;
    stack::pop(&mainCPU->stack, &b);
    stack::pop(&mainCPU->stack, &a);
    stack::push(&mainCPU->stack, a / b);
    }
)
DESCRIPT_COMMAND("out", 0x04, {
    AsmCode_t a = 0;
    stack::pop(&mainCPU->stack, &a);
    printf("%u\n", a);
    }
)
DESCRIPT_COMMAND("pop", 0x05, {
    stack::pop(&mainCPU->stack, commandArgs.argWritePtr);
    }
)
DESCRIPT_COMMAND("jmp", 0x06, {
    mainCPU->code.pc = commandArgs.argSum;
    }
)
#define DESCRIPT_JMP_COMMAND(name, bytecode, _) \
DESCRIPT_COMMAND(name, bytecode, {               \
    AsmCode_t a = 0;                              \
    AsmCode_t b = 0;                               \
    stack::pop(&mainCPU->stack, &b);                \
    stack::pop(&mainCPU->stack, &a);                 \
    if (a _ b) {                                      \
        mainCPU->code.pc = commandArgs.argSum;         \
        }                                               \
    }                                                    \
)

DESCRIPT_JMP_COMMAND("ja",  0x07, >  )
DESCRIPT_JMP_COMMAND("jae", 0x08, >= )
DESCRIPT_JMP_COMMAND("jb",  0x09, <  )
DESCRIPT_JMP_COMMAND("jbe", 0x0A, <= )
DESCRIPT_JMP_COMMAND("je",  0x0B, == )
DESCRIPT_JMP_COMMAND("jne", 0x0C, != )
#undef DESCRIPT_JMP_COMMAND


DESCRIPT_COMMAND("call", 0x0D, {
    stack::push(&mainCPU->funcStack, mainCPU->code.pc);
    mainCPU->code.pc = commandArgs.argSum;
    }
)
DESCRIPT_COMMAND("ret", 0x0E, {
    AsmCode_t a = 0;
    stack::pop(&mainCPU->funcStack, &a);
    mainCPU->code.pc = a;
    }
)
DESCRIPT_COMMAND("mul", 0x0F, {
    AsmCode_t a = 0;
    AsmCode_t b = 0;
    stack::pop(&mainCPU->stack, &b);
    stack::pop(&mainCPU->stack, &a);
    stack::push(&mainCPU->stack, a * b);
    }
)
