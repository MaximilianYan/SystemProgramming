#include <stdio.h>
#include <string.h>

#include "tests.h"
#include "compareDouble.h"

#include "strLib.h"

#define SCAN(STR, ADDR)                                \
{                                                      \
        if (fscanf(inputFile, STR, ADDR) < 1) {        \
        return TestResult::INPUT_ERROR;                \
    }                                                  \
}

#define SCANREAD(ADDR, N)                              \
{                                                      \
        if ((int)fread(ADDR, 1, N, inputFile) < (N)) { \
        return TestResult::INPUT_ERROR;                \
    }                                                  \
}

#define FUNC_TO_TEST_LIST(FUNC) FUNC, #FUNC

namespace test {
    /**
     * @brief Tests function
     *
     * **Function**:
     * int strLib::strlen(const char* str);
     *
     * @param [in] inputFile File with test
     * @return TestResult Result of testing
     */
    TestResult strLib_strlen(FILE* inputFile) {
        struct {
            char str[MAX_FUNC_NAME_LENGTH];
            int resultAns;
        } params = {};

        // ------------- read params -------------

        int strKey = 0;
        SCAN(" [%d]", &strKey);
        SCANREAD(params.str, strKey);

        SCAN("%d", &params.resultAns);

        // ------------- read params -------------
        // test function

        int result = strLib::strlen(params.str);

        if (
            (result == params.resultAns)) {
            return TestResult::PASSED;
        }

        printf("TEST FAILED : %s: input: %s\n"
            "       expected output: %d\n"
            "       got      output: %d\n",
            "strLib_strlen", params.str,
            params.resultAns,
            result);
        return TestResult::FAILED;
    }

    /**
     * @brief Tests function
     *
     * **Function**:
     * char* strLib::strncpy(char* dst, const char* src, int count);
     *
     * @param [in] inputFile File with test
     * @return TestResult Result of testing
     */
    TestResult strLib_strncpy(FILE* inputFile) {
        struct {
            char src[MAX_FUNC_NAME_LENGTH];
            char dst[MAX_FUNC_NAME_LENGTH];
            int count;
            char* resultAns;
            char dstAns[MAX_FUNC_NAME_LENGTH];
        } params = {};

        // ------------- read params -------------

        int srcKey = 0;
        SCAN(" [%d]", &srcKey);
        SCANREAD(params.src, srcKey);

        SCAN("%d", &params.count);
        // resultAns = dst

        int dstAnsKey = 0;
        SCAN(" [%d]", &dstAnsKey);
        SCANREAD(params.dstAns, dstAnsKey);

        // ------------- read params -------------
        // test function

        params.resultAns = params.dst;
        char* result = strLib::strncpy(params.dst, params.src, params.count);

        if (
            (result == params.resultAns) &&
            (strcmp(params.dst, params.dstAns) == 0) &&
            (params.dst[params.count] == '\0')) {
            return TestResult::PASSED;
        }

        printf("TEST FAILED : %s: input: %s, %d\n"
            "       expected output: %s, %s\n"
            "       got      output: %s, %s\n",
            "strLib_strncpy", params.src, params.count,
            params.dstAns, params.resultAns,
            params.dst, result);
        return TestResult::FAILED;
    }

    /**
     * @brief Tests function
     *
     * **Function**:
     * char* strLib::strncat(char* dst, const char* src, int count);
     *
     * @param [in] inputFile File with test
     * @return TestResult Result of testing
     */
    TestResult strLib_strncat(FILE* inputFile) {
        struct {
            char src[MAX_FUNC_NAME_LENGTH];
            char dst[MAX_FUNC_NAME_LENGTH];
            int count;
            char* resultAns;
            char dstAns[MAX_FUNC_NAME_LENGTH];
        } params = {};

        // ------------- read params -------------

        int srcKey = 0;
        SCAN(" [%d]", &srcKey);
        SCANREAD(params.src, srcKey);

        int dstKey = 0;
        SCAN(" [%d]", &dstKey);
        SCANREAD(params.dst, dstKey);

        SCAN("%d", &params.count);
        // resultAns = dst

        int dstAnsKey = 0;
        SCAN(" [%d]", &dstAnsKey);
        SCANREAD(params.dstAns, dstAnsKey);

        // ------------- read params -------------
        // test function

        params.resultAns = params.dst;
        char* result = strLib::strncat(params.dst, params.src, params.count);

        if (
            (result == params.resultAns) &&
            (strcmp(params.dst, params.dstAns) == 0)) {
            return TestResult::PASSED;
        }

        printf("TEST FAILED : %s: input: %s, %d\n"
            "       expected output: %s, %s\n"
            "       got      output: %s, %s\n",
            "strLib_strncat", params.src, params.count,
            params.dstAns, params.resultAns,
            params.dst, result);
        return TestResult::FAILED;
    }

    /**
     * @brief Tests function
     *
     * **Function**:
     * const char* strLib::strstr(const char* haystack, const char* needle);
     *
     * @param [in] inputFile File with test
     * @return TestResult Result of testing
     */
    TestResult strLib_strstr(FILE* inputFile) {
        struct {
            char haystack[MAX_FUNC_NAME_LENGTH];
            char needle[MAX_FUNC_NAME_LENGTH];
            char* resultAns;
        } params = {};

        // ------------- read params -------------

        int haystackKey = 0;
        SCAN(" [%d]", &haystackKey);
        SCANREAD(params.haystack, haystackKey);

        int needleKey = 0;
        SCAN(" [%d]", &needleKey);
        SCANREAD(params.needle, needleKey);

        int resultAnsKey = 0;
        SCAN("%d", &resultAnsKey);
        if (resultAnsKey == -1) {
            params.resultAns = nullptr;
        } else {
            params.resultAns = params.haystack + resultAnsKey;
        }

        // ------------- read params -------------
        // test function

        const char* result = strLib::strstr(params.haystack, params.needle);

        if (
            (result == params.resultAns)) {
            return TestResult::PASSED;
        }

        printf("TEST FAILED : %s: input: %s, %s\n"
            "       expected output: %s\n"
            "       got      output: %s\n",
            "strLib_strstr", params.haystack, params.needle,
            params.resultAns,
            result);
        return TestResult::FAILED;
    }

    /**
     * @brief Tests function
     *
     * **Function**:
     * char* strLib::strtok(char* str, const char* delim);
     *
     * @param [in] inputFile File with test
     * @return TestResult Result of testing
     */
    TestResult strLib_strtok(FILE* inputFile) {
        struct {
            char str[MAX_FUNC_NAME_LENGTH];
            char delim[MAX_FUNC_NAME_LENGTH];
            char* resultAns;
        } params = {};

        // ------------- read params -------------

        int strKey = 0;
        SCAN(" [%d]", &strKey);
        SCANREAD(params.str, strKey);

        int delimKey = 0;
        SCAN(" [%d]", &delimKey);
        SCANREAD(params.delim, delimKey);

        int resultAnsKey = 0;
        SCAN("%d", &resultAnsKey);
        if (resultAnsKey == -1) {
            params.resultAns = nullptr;
        } else {
            params.resultAns = params.str + resultAnsKey;
        }

        // ------------- read params -------------
        // test function

        char* result = strLib::strtok(params.str, params.delim);

        ///@warning Do not test ' \ 0 ' replacing
        if (
            (result == params.resultAns)) {
            return TestResult::PASSED;
        }

        printf("TEST FAILED : %s: input: %s, %s\n"
            "       expected output: %s\n"
            "       got      output: %s\n",
            "strLib_strtok", params.str, params.delim,
            params.resultAns,
            result);
        return TestResult::FAILED;
    }

    /**
     * @brief Tests function
     *
     * **Function**:
     * char* strLib::strdup(const char* str1);
     *
     * @param [in] inputFile File with test
     * @return TestResult Result of testing
     */
    TestResult strLib_strdup(FILE* inputFile) {
        struct {
            char str1[MAX_FUNC_NAME_LENGTH];
            char* resultAns;
        } params = {};

        // ------------- read params -------------

        int str1Key = 0;
        SCAN(" [%d]", &str1Key);
        SCANREAD(params.str1, str1Key);

        // *resultAns must be = *str1
        params.resultAns = params.str1;

        // ------------- read params -------------
        // test function

        char* result = strLib::strdup(params.str1);

        if (
            (strcmp(result, params.resultAns) == 0)) {
            free(result);
            return TestResult::PASSED;
        }

        printf("TEST FAILED : %s: input: %s\n"
            "       expected output: %s\n"
            "       got      output: %s\n",
            "strLib_strdup", params.str1,
            params.resultAns,
            result);

        free(result);
        return TestResult::FAILED;
    }

    const TestFuncInfo testFuncList[NUMBER_OF_TEST_FUNCS] = {
        { FUNC_TO_TEST_LIST(strLib_strlen)  },
        { FUNC_TO_TEST_LIST(strLib_strncpy) },
        { FUNC_TO_TEST_LIST(strLib_strncat) },
        { FUNC_TO_TEST_LIST(strLib_strstr)  },
        { FUNC_TO_TEST_LIST(strLib_strtok)  },
        { FUNC_TO_TEST_LIST(strLib_strdup)  },
    };
}
