/**
 * @file cpu.h
 * @author Maximilian (https://github.com/MaximilianYan)
 * @brief Soft CPU code
 * @date 2022-10-08
 *
 * @copyright Copyright (c) 2022
 *
 */

#ifndef CPU_H
#define CPU_H

#include <..\..\004.Stack\StackLib\StackLibWorld.h>
#include <..\AsmCode.h>

#ifndef NDEBUG
#ifdef CPU_DEBUG
#else // !STACK_DEBUG
#define CPU_DEBUG
#endif // STACK_DEBUG
#endif // NDEBUG

struct CPU {
    AsmCode code;
    Stack stack;

    enum MODE {
        RUNCPU = 0,   ///< Normal mode
        DISASSEMBLER, ///< Disassembler mode
    };
    MODE mode;

#ifdef CPU_DEBUG
    DebugInfo debugInfo;
#endif // CPU_DEBUG
};

namespace cpu {
    /**
     * @brief Codes for errors in returned values
     *
     */
    enum Error {
        OK = 0,          ///< No errors, program had to continue
        OK_HALT,         ///< No errors, program had to stop
        UNREACHABLE_HLT, ///< Unreacheble point in program is reached. !!! Sth. has gone VARY wrong
        UNKNOWN_COMMAND, ///< Code error: unknown command
    };
    Error run(CPU* mainCPU);
    Error runCommand(CPU* mainCPU);

#ifdef CPU_DEBUG
#define CPU__ctor(obj) cpu::ctor(&obj, DEBUGINFO_CTOR_ARGS_R(#obj)
    Error ctor(CPU* const cpu, DEBUGINFO_CTOR_ARGS_H, CPU::MODE mode = CPU::MODE::RUNCPU, size_t codeBufferSize = 0);
#else // !STACK_DEBUG
#define CPU__ctor(obj) cpu::ctor(&obj
    Error ctor(CPU* const cpu, CPU::MODE mode, size_t codeBufferSize = 0);
#endif // STACK_DEBUG

    Error dtor(CPU* const cpu);

#define CPU__dump(stackObj) cpu::dump(&stackObj, LOGFUNCHEAD_ARGS_R
    Error dump(CPU* const cpu, LOGFUNCHEAD_ARGS_H);
}

#endif // CPU_H
