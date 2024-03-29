#include <string.h>
#include <assert.h>
#include <ctype.h>

#include "strAsmLang.h"

namespace strAsmLang {
    /**
     * @brief Parse commandName from line
     *
     * @param [in] line Line
     * @param [out] buffer Buffer for commandName
     * @param [out] length Got commanName's length (excluding \0)
     * @param [in] maxLength Max size of buffer
     * @return Error Error code
     */
    Error getCommandName(const Line* line, char* buffer, int* length, int maxLength) {
        assert(line != nullptr);
        assert(buffer != nullptr);
        assert(length != nullptr);

        int index = 0;
        do {
            if (index >= maxLength)
                return Error::BUF_OVERFLOW;
            if (index > line->lenght)
                return Error::BROKEN_LINE;

            buffer[index] = line->str[index];

            if (buffer[index] == '@')
                buffer[index] = '\0';

            if (buffer[index] == '\0')
                break;
        } while (++index);

        *length = index;

        return Error::OK;
    }

    /**
     * @brief If name is a label's name
     *
     * @param name Name string
     * @param length Name string's length (excluding \0)
     * @return true
     * @return false
     */
    bool isNameLabel(char const* const name, const int length) {
        assert(name != nullptr);
        assert(length >= 0);
        return (
            name[0] == ':' ||
            name[length - 1] == ':'
            );
    }

    /**
     * @brief Skip correct symmbols
     *
     * @param [in] line Line
     * @param [out] shift Position in line
     * @param [in] isCorrect Classifier of symbols
     * @return Error Error code
     */
    Error skipSymbols(const Line* line, int* shift, bool isCorrect(char symb)) {
        assert(line != nullptr);
        assert(shift != nullptr);
        assert(isCorrect != null);

        char symb = 0;
        do {
            if (*shift > line->lenght)
                return Error::BROKEN_LINE;

            symb = line->str[*shift];

            if (symb == '\0')
                break;

            if (!isCorrect(symb))
                break;
        } while (++(*shift));

        return Error::OK;
    }

    /**
     * @brief Classifier for **skipSymbols** for Immidiate const and Memory arguments
     *
     * @param [in] symb Symbol
     */
    bool isCorrectSymbI(char symb) {
        return isdigit(symb);
    }

    /**
     * @brief Classifier for **skipSymbols** for Register arguments
     *
     * @param [in] symb Symbol
     */
    bool isCorrectSymbR(char symb) {
        return (
            symb == 'r' ||
            symb == 'a' ||
            symb == 'b' ||
            symb == 'c' ||
            symb == 'd' ||
            symb == 'x');
    }

    /**
     * @brief Classifier for **skipSymbols** for Memory brackets
     *
     * @param [in] symb Symbol
     */
    bool isCorrectSymbM(char symb) {
        return (
            symb == '[' ||
            symb == ']');
    }

    /**
     * @brief Returns code of the argument type
     *
     * @param [in] symb Symbol
     * @return AsmCode_t Argument type code or **null** if error is occured
     */
    AsmCode_t classifyArg(char symb) {
        if (isdigit(symb))
            return asmLang::COMMAND_ARG_HAS_I;
        if (symb == 'r')
            return asmLang::COMMAND_ARG_HAS_R;
        if (symb == '[')
            return asmLang::COMMAND_ARG_HAS_M;
        if (symb == ']')
            return asmLang::COMMAND_ARG_HAS_M;

        return null;
    }

    /**
     * @brief Returns code of register
     *
     * @param [in] symb Symbol
     * @return AsmCode_t Register type or **null** if error is occured
     */
    AsmCode_t classifyReg(char symb) {
        switch (symb) {
#define PR_REGISTER(name, code, ...) \
        case #name[0]:                \
            return code;
#include <..\asmLangDSLRegisters.cpp>
        default:
            return null;
        }
#undef PR_REGISTER
    }

    int sscanAsmCode_t(char const* str, AsmCode_t* const a) {
        assert(str != nullptr);
        assert(a != nullptr);

        long long buf = 0l;
        bool isNegative = false;

        if (*str == '\0')
            return 0;
        if (*str == '-') {
            isNegative = true;
            str++;
        }
        while (isdigit(*str)) {
            buf *= 10;
            buf += (long long)(*str - '0');
            str++;
        }

        *a = (AsmCode_t)(isNegative ? -buf : buf);
        return 1;
    }
}
