#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "cyrillicString.h"

namespace cyrillicString {
    static const ListOfLines* linesOfAlphabet = nullptr;
    static bool isAlphabetInitialized = false;

    /**
     * @brief Set the alphabet with its ListOfLines
     *
     * @param [in] linesOfAlphabetPtr
     */
    void setAlphabet(const ListOfLines* linesOfAlphabetPtr) {
        assert(linesOfAlphabetPtr != nullptr);
        
        cyrillicString::linesOfAlphabet = linesOfAlphabetPtr;
        isAlphabetInitialized = false;
    }

    /**
     * @brief Get the index of cyrillic character
     *
     * Can be used for comparing
     *
     * @param [in] a Character
     * @return int Index
     */
    int getCyrillicIndex(const char a) {
        static int alphabetTable[256] = { 0 };

        assert(linesOfAlphabet != nullptr);

        // Мы с научником вернёмся к этой задаче через 2 года 16.09.2024 и я скажу, что думаю об этом решении
        // Ну вот теперь решение получше (19.09.2022)
        if (isAlphabetInitialized == false) {
            isAlphabetInitialized = true;

            for (int symbI = 0; symbI < 256; ++symbI) {
                alphabetTable[symbI] = BAD_CHAR;
            }
            for (int i = 0; i < linesOfAlphabet->size; ++i) {
                alphabetTable[(unsigned char)linesOfAlphabet->lines[i].str[0]] = i;
            }
        }

        return alphabetTable[(unsigned char)a];
    }

    /**
     * @brief Comparator for cyrillic characters
     *
     * @param a
     * @param b
     * @return CmpCyrillicResult
     */
    CmpCyrillicResult cmpCyrillic(const char a, const char b) {
        assert(linesOfAlphabet != nullptr);

        int aIndex = getCyrillicIndex(a);
        int bIndex = getCyrillicIndex(b);

        if (aIndex == BAD_CHAR && bIndex != BAD_CHAR) {
            return CmpCyrillicResult::BAD_A;
        }
        if (aIndex != BAD_CHAR && bIndex == BAD_CHAR) {
            return CmpCyrillicResult::BAD_B;
        }
        if (aIndex == BAD_CHAR && bIndex == BAD_CHAR) {
            return CmpCyrillicResult::BAD_BOTH;
        }
        if (aIndex < bIndex) {
            return CmpCyrillicResult::LESSER;
        }
        if (aIndex > bIndex) {
            return CmpCyrillicResult::GREATER;
        }
        return CmpCyrillicResult::EQV;
    }

    /**
     * @brief Comparator for cyrillic lines
     *
     * @param a
     * @param b
     * @param dir
     * @param aLength
     * @param bLength
     * @return int -1 / 0 / 1
     */
    int cmpLinesStr(const char* a, const char* b, CmpDirection dir, int aLength, int bLength) {
        assert(a != nullptr);
        assert(b != nullptr);
        switch (dir) {
        case CmpDirection::RIGHT:
            assert(aLength >= 0);
            assert(bLength >= 0);
            break;
        case CmpDirection::REVERSE:
            assert(aLength > 0);
            assert(bLength > 0);
            break;
        default:
            assert(false && "CmpDirection is what?");
            break;
        }

        int iA = (dir == CmpDirection::RIGHT) ? 0 : aLength;
        int iB = (dir == CmpDirection::RIGHT) ? 0 : bLength;
        while (true) {
            if (dir == CmpDirection::RIGHT) {
                if (a[iA] == '\0') {
                    if (b[iB] == '\0') return  0;
                    else               return -1;
                }
                if (b[iB] == '\0')
                    return 1;
            } else  { // dir == CmpDirection::REVERSE
                if (iA == -1) {
                    if (iB == -1)      return  0;
                    else               return -1;
                }
                if (iB == -1)
                    return 1;
            }

            switch (cmpCyrillic(a[iA], b[iB])) {
                case CmpCyrillicResult::BAD_A:
                    iA += dir;
                    continue;
                case CmpCyrillicResult::BAD_B:
                    iB += dir;
                    continue;
                case CmpCyrillicResult::BAD_BOTH:
                    iA += dir;
                    iB += dir;
                    continue;

                case CmpCyrillicResult::LESSER:
                    return -1;
                case CmpCyrillicResult::GREATER:
                    return 1;
                case CmpCyrillicResult::EQV:
                    iA += dir;
                    iB += dir;
                    continue;
                default:
                    assert(false && "cmpCyrillic returned what?");
            }
        }

        return 0;
    }

    /**
     * @brief Type independed comparator for cyrillic lines
     *
     * @param a (void*)charPtr
     * @param b (void*)charPtr
     * @return int
     */
    int cmpLines(const void* a, const void* b) {
        assert(a != nullptr);
        assert(b != nullptr);
        return cmpLinesStr(((const Line*)a)->str, ((const Line*)b)->str, CmpDirection::RIGHT);
    }

    /**
     * @brief Type independed back omparator for cyrillic lines
     *
     * @param a (void*)charPtr
     * @param b (void*)charPtr
     * @return int
     */
    int cmpLinesBack(const void* a, const void* b) {
        assert(a != nullptr);
        assert(b != nullptr);
        return cmpLinesStr(((const Line*)a)->str, ((const Line*)b)->str, CmpDirection::REVERSE, ((const Line*)a)->lenght, ((const Line*)b)->lenght);
    }
}

void memswap(void* aVoid, void* bVoid, size_t size); ///< Fast memory swapping

void memswap(void* aVoid, void* bVoid, size_t size) {
    assert(aVoid != nullptr);
    assert(bVoid != nullptr);

    char* aPtr = (char*)aVoid;
    char* bPtr = (char*)bVoid;
    size_t sizeDone = 0;

    uint64_t tmp64 = 0;
    while ((sizeDone - size) >> sizeof(uint64_t)) {
        tmp64                         = *(uint64_t*)(aPtr + sizeDone);
        *(uint64_t*)(aPtr + sizeDone) = *(uint64_t*)(bPtr + sizeDone);
        *(uint64_t*)(bPtr + sizeDone) = tmp64;

        sizeDone += sizeof(uint64_t);
    }

    uint32_t tmp32 = 0;
    while ((sizeDone - size) >> sizeof(uint32_t)) {
        tmp32                         = *(uint32_t*)(aPtr + sizeDone);
        *(uint32_t*)(aPtr + sizeDone) = *(uint32_t*)(bPtr + sizeDone);
        *(uint32_t*)(bPtr + sizeDone) = tmp32;

        sizeDone += sizeof(uint32_t);
    }

    uint16_t tmp16 = 0;
    while ((sizeDone - size) >> sizeof(uint16_t)) {
        tmp16                         = *(uint16_t*)(aPtr + sizeDone);
        *(uint16_t*)(aPtr + sizeDone) = *(uint16_t*)(bPtr + sizeDone);
        *(uint16_t*)(bPtr + sizeDone) = tmp16;

        sizeDone += sizeof(uint16_t);
    }

    uint8_t tmp8 = 0;
    while ((sizeDone - size) >> sizeof(uint8_t)) {
        tmp8                         = *(uint8_t*)(aPtr + sizeDone);
        *(uint8_t*)(aPtr + sizeDone) = *(uint8_t*)(bPtr + sizeDone);
        *(uint8_t*)(bPtr + sizeDone) = tmp8;

        sizeDone += sizeof(uint8_t);
    }
}

/**
 * @brief Quick sort algorithm
 *
 * @param arr Array
 * @param begin Begin index
 * @param end End index
 * @param cmp Compare function
 * @param size sizeof(element)
 */
void uq_sort(void* arr, int begin, int end, int(*cmp)(const void*, const void*), int size) {
    if (begin >= end) {
        return;
    }

    int baseI = (begin + end) / 2;

    int l = begin;
    int r = end;

    void* tmp = calloc(1, size);
    assert(tmp != nullptr);

    while (l < r) {
        while (cmp((char*)arr + l * size, (char*)arr + baseI * size) == -1) ++l;
        while (cmp((char*)arr + r * size, (char*)arr + baseI * size) == +1) --r;

        if (l >= r) {
            break;
        }

        // swap
        if (l == baseI) {
            baseI = r;
        } else if (r == baseI) {
            baseI = l;
        }
        memswap((char*)arr + l * size, (char*)arr + r * size, size);

        // Ифы имени Миши
        if (l != baseI)
            ++l;
        if (r != baseI)
            --r;
    }

    free(tmp);

    for (int i = begin; i <= baseI; ++i) {
        assert(cmp((char*)arr + i * size, (char*)arr + baseI * size) <= 0 || (printf("%d %d", i, baseI), false));
    }
    for (int i = baseI; i <= end; ++i) {
        assert(cmp((char*)arr + i * size, (char*)arr + baseI * size) >= 0 || (printf("%d %d", i, baseI), false));
    }

    uq_sort(arr, begin, baseI - 1, cmp, size);
    uq_sort(arr, baseI + 1,   end, cmp, size);
}
