/**
 * @file Stack.h
 * @author Maximilian (https://github.com/MaximilianYan)
 * @brief Stack implementation
 * @date 2022-09-23
 *
 * @copyright Copyright (c) 2022
 *
 */

#ifndef STACK_H
#define STACK_H

#include <stdlib.h>

#include "..\logger\logger.h"
#include "..\DebugInfo\DebugInfo.h"

typedef int Elem_t;

/*
#define STACK_DEBUG
#ifndef STACK_DEBUG
#else // !STACK_DEBUG
#endif // STACK_DEBUG
*/

#ifdef STACK_DEBUG
class Stack : public Debuggable {
#else // !STACK_DEBUG
class Stack {
#endif // STACK_DEBUG
public:
    Elem_t* data;
    size_t size;
    size_t capacity;
};

namespace stack {
    enum Error {
        OK = 0,
        EMPTY,
        DATA_TRUNC,
    };

#ifdef STACK_DEBUG
#define STACK__ctor(obj) stack::ctor(&obj, DEBUGINFO_CTOR_ARGS_R(#obj)
    Error ctor(Stack* const stack, DEBUGINFO_CTOR_ARGS_H, size_t capacity = 0);
#else // !STACK_DEBUG
#define STACK__ctor(obj) stack::ctor(&obj
    Error ctor(Stack* const stack, size_t capacity = 0);
#endif // STACK_DEBUG

    Error dtor(Stack* const stack);

    Error resize(Stack* const stack, size_t newCapacity);

    size_t getSize(Stack* const stack);

    Error push(Stack* const stack, Elem_t elem);
    Error pop(Stack* const stack, Elem_t* const dst = nullptr);
    Error getLast(Stack* const stack, Elem_t* const dst);

#define STACK__dump(stackObj) stack::dump(&stackObj, LOGFUNCHEAD_ARGS_R
    Error dump(Stack* const stack, LOGFUNCHEAD_ARGS_H);
}

#endif // STACK_H
