#include <assert.h>
#include <stdio.h>
#include <string.h>

#include "loggerList.h"

#include "List.h"

#define VERIFY(obj) if (verify(obj)) { LIST__dump(*obj)); return Error::VERIFIER_ERR; }

/**
 * @brief List constructor
 *
 * @param [out] list List
 * @param [in] elemSize Size of an element
 * @param [in] outFunc Pointer to a function for printing elements'es value
 * @return List::Error Error code
 */
List::Error List::ctor(List* const list, DEBUGINFO_CTOR_ARGS_H, size_t const elemSize, ValueOutFunction_t outFunc, size_t capacity) {
    assert(list);

    list->debugInfo.objName = objName;
    list->debugInfo.ctorCallLine = ctorCallLine;
    list->debugInfo.ctorCallFile = ctorCallFile;
    list->debugInfo.ctorCallFunc = ctorCallFunc;

    list->freeTail = 0;
    list->size = 0;
    list->capacity = 0;

    list->elemSize = elemSize;
    list->outFunc = outFunc;

    list->bufferElem = (ListElem*)calloc(1, sizeof(ListElem));
    list->bufferValue = calloc(1, list->elemSize);
    if (!list->bufferElem) return Error::MEM_ERR;
    if (!list->bufferValue) return Error::MEM_ERR;

    resize(list, capacity);

    // VERIFY(list);
    return Error::OK;
}

/**
 * @brief List destructor
 *
 * @param [out] list List
 * @return List::Error Error code
 */
List::Error List::dtor(List* const list) {
    assert(list);

    list->freeTail = 0;
    list->size = 0;
    list->capacity = 0;

    /*while (!List::isEmpty(list)) {
        popBack(list);
    }*/
    free(list->bufferElem);
    free(list->bufferValue);

    list->bufferElem = nullptr;
    list->bufferValue = nullptr;

    list->debugInfo.objName = "ZZZOMBIE";

    return Error::OK;
}

/**
 * @brief Resize list's buffer
 *
 * @param [out] list List
 * @param [int] newCapacity New capacity of list
 * @return List::Error Error code
 */
List::Error List::resize(List* const list, size_t newCapacity) {
    assert(list);

    if (newCapacity < list->size)
        newCapacity = list->size;

    ListElem* newBufferElem = (ListElem*)calloc(newCapacity + 1, sizeof(ListElem));
    void* newBufferValue = calloc(newCapacity + 1, list->elemSize);

    for (size_t index = 1; index < newCapacity + 1; ++index) {
        ListElem* elem = &newBufferElem[index];

        if (index > 1)
            elem->prev = index - 1;
        else
            elem->prev = 0;
        if (index != newCapacity)
            elem->next = index + 1;
        else
            elem->next = 0;
    }

    if (list->size) {
        assert(list->size <= list->capacity);

        ListIterator iterator;
        List::begin(list, &iterator);
        uint8_t* elemValue = (uint8_t*)newBufferValue;
        do {
            elemValue += list->elemSize;
            memcpy(elemValue, ListIterator::getValue(list, &iterator), list->elemSize);
        } while (!ListIterator::next(list, &iterator));

        newBufferElem[list->size].next = 0;
        newBufferElem[list->size + 1].prev = list->size;

        if (list->size < list->capacity) {
            list->freeTail = list->size + 1;
        } else {
            list->freeTail = 0;
        }
    } else {
        list->freeTail = 1;
        newBufferElem[0].prev = 0;
        newBufferElem[0].next = 0;
    }

    free(list->bufferElem);
    free(list->bufferValue);
    list->bufferElem = newBufferElem;
    list->bufferValue = newBufferValue;
    list->capacity = newCapacity;

    return Error::OK;
}

#define SET_NEW_ELEMENT                             \
    newElem->valuePtr = calloc(list->elemSize, 1);   \
    if (!newElem->valuePtr) return Error::MEM_ERR;    \
    if (src)                                           \
        memcpy(newElem->valuePtr, src, list->elemSize); \
    else                                                 \
        memset(newElem->valuePtr, 0, list->elemSize);

/**
 * @brief Push new element to the back of the list
 *
 * @param [out] list List
 * @param [in] src Pointer to the new element value
 * @return List::Error Error code
 */
List::Error List::pushBack(List* const list, void const* const src) {
    assert(list);

    size_t newElem = list->freeTail;

    if (list->size) {
        assert(list->bufferElem[0].prev);
        assert(list->bufferElem[0].next);
        assert(list->bufferElem[list->bufferElem[0].prev].next == 0);

        list->bufferElem[list->bufferElem[0].prev].next = newElem;
        list->bufferElem[newElem].prev = list->bufferElem[0].prev;
    } else {
        assert(!list->bufferElem[0].prev);
        assert(!list->bufferElem[0].next);
        list->bufferElem[0].next = newElem;
        list->bufferElem[newElem].prev = 0;
    }
    list->bufferElem[0].prev = newElem;
    list->freeTail = list->bufferElem[newElem].next;
    list->bufferElem[list->freeTail].prev = 0;
    list->bufferElem[newElem].next = 0;
    list->size++;

    // SET_NEW_ELEMENT;
    if (src)
        memcpy((uint8_t*)list->bufferValue + list->elemSize * newElem, src, list->elemSize);
    else
        memset((uint8_t*)list->bufferValue + list->elemSize * newElem, 0, list->elemSize);

    return Error::OK;
}

/**
 * @brief Push new element to the front of the list
 *
 * @param [out] list List
 * @param [in] src Pointer to the new element value
 * @return List::Error Error code
 */
List::Error List::pushFront(List* const list, void const* const src) { /*
    assert(list);

    MAKE_NEW_ELEMENT;

    if (list->tail) {
        assert(list->head);
        assert(list->tail->prev == nullptr);

        list->tail->prev = newElem;
        newElem->next = list->tail;
    } else {
        assert(!list->head);
        list->head = newElem;
    }
    list->tail = newElem;

    SET_NEW_ELEMENT_VALUE;*/

    return Error::OK;
}

/**
 * @brief Pop an element from the back of the list
 *
 * @param [out] list List
 * @return List::Error Error code
 */
List::Error List::popBack(List* const list) {/*
    assert(list);

    if (!list->head) {
        assert(!list->tail);
        return Error::EMPTY;
    }
    assert(list->tail);

    assert(!list->head->next);
    ListElem* elem = list->head;

    free(elem->valuePtr);
    elem->valuePtr = nullptr;

    if (elem->prev) {
        list->head = elem->prev;
        list->head->next = nullptr;
    } else {
        assert(list->head == list->tail);
        list->head = nullptr;
        list->tail = nullptr;
    }
    free(elem);*/

    return Error::OK;
}

/**
 * @brief Pop an element from the front of the list
 *
 * @param [out] list List
 * @return List::Error Error code
 */
List::Error List::popFront(List* const list) {/*
    assert(list);

    if (!list->head) {
        assert(!list->tail);
        return Error::EMPTY;
    }
    assert(list->tail);

    assert(!list->tail->prev);
    ListElem* elem = list->tail;

    free(elem->valuePtr);
    elem->valuePtr = nullptr;

    if (elem->next) {
        list->tail = elem->next;
        list->tail->prev = nullptr;
    } else {
        assert(list->head == list->tail);
        list->head = nullptr;
        list->tail = nullptr;
    }
    free(elem);*/

    return Error::OK;
}

/**
 * @brief Add new element after the element of list
 *
 * @param [out] list List
 * @param [in] iterator Iterator to the element
 * @param [in] src Pointer to the new element value
 * @return List::Error Error code
 */
List::Error List::emplaceAfter(List* const list, ListIterator const* const iterator, void const* const src) {/*
    assert(list);
    assert(iterator);

    Error err = OK;
    if (iterator->ptr->next == nullptr) {
        err = pushBack(list, src);
        return err;
    }

    MAKE_NEW_ELEMENT;

    assert(iterator->ptr->next->prev == iterator->ptr);

    newElem->prev = iterator->ptr;
    newElem->next = iterator->ptr->next;

    iterator->ptr->next->prev = newElem;
    iterator->ptr->next = newElem;

    SET_NEW_ELEMENT_VALUE;*/

    return Error::OK;
}

/**
 * @brief Add new element before the element of list
 *
 * @param [out] list List
 * @param [in] iterator Iterator to the element
 * @param [in] src Pointer to the new element value
 * @return List::Error Error code
 */
List::Error List::emplaceBefore(List* const list, ListIterator const* const iterator, void const* const src) {/*
    assert(list);
    assert(iterator);

    Error err = OK;
    if (iterator->ptr->prev == nullptr) {
        err = pushFront(list, src);
        return err;
    }

    MAKE_NEW_ELEMENT;


    assert(iterator->ptr->prev->next == iterator->ptr);

    newElem->next = iterator->ptr;
    newElem->prev = iterator->ptr->prev;

    iterator->ptr->prev->next = newElem;
    iterator->ptr->prev = newElem;

    SET_NEW_ELEMENT_VALUE;*/

    return Error::OK;
}

/**
 * @brief Add new element to the list
 *
 * @param [out] list List
 * @param [in] iterator Iterator to the element
 * @param [in] direction Does need emplace BEFORE or AFTER the element
 * @param [in] src Pointer to the new element value
 * @return List::Error Error code
 */
List::Error List::emplace(List* const list, ListIterator const* const iterator, Direction direction, void const* const src) {
    switch (direction) {
    case Direction::FORWARD:
        return emplaceAfter(list, iterator, src);
    case Direction::BACKWARD:
        return emplaceBefore(list, iterator, src);
    default:
        assert(0);
    }

    return Error::OK;
}


/**
 * @brief Erase element from the list
 *
 * @param [out] list List
 * @param [out] iterator Iterator to the element
 * @param [in] direction If iterator will be set to the next or previous element (**nullptr** if there is no such an element)
 * @return List::Error Error code
 */
List::Error List::erase(List* const list, ListIterator* const iterator, Direction direction) {/*
    assert(list);
    assert(iterator);

    Error err = OK;
    if (iterator->ptr->next == nullptr) {
        err = popBack(list);

        switch (direction) {
        case Direction::FORWARD:
            iterator->ptr = nullptr;
            break;
        case Direction::BACKWARD:
            List::rbegin(list, iterator);
            break;
        default:
            assert(0);
        }

        return err;
    }
    if (iterator->ptr->prev == nullptr) {
        err = popFront(list);
        iterator->ptr = nullptr;

        switch (direction) {
        case Direction::FORWARD:
            List::begin(list, iterator);
            break;
        case Direction::BACKWARD:
            iterator->ptr = nullptr;
            break;
        default:
            assert(0);
        }

        return err;
    }


    assert(iterator->ptr->prev->next == iterator->ptr);
    assert(iterator->ptr->next->prev == iterator->ptr);
    iterator->ptr->prev->next = iterator->ptr->next;
    iterator->ptr->next->prev = iterator->ptr->prev;

    ListElem* elemToSet;
    switch (direction) {
    case Direction::FORWARD:
        elemToSet = iterator->ptr->next;
        break;
    case Direction::BACKWARD:
        elemToSet = iterator->ptr->prev;
        break;
    default:
        assert(0);
    }

    free(iterator->ptr->valuePtr);
    iterator->ptr->valuePtr = nullptr;
    free(iterator->ptr);

    iterator->ptr = elemToSet;*/

    return Error::OK;
}

#undef MAKE_NEW_ELEMENT
#undef SET_NEW_ELEMENT_VALUE

/**
 * @brief Is list empty
 *
 * @param [in] list List
 */
bool List::isEmpty(List const* const list) {
    assert(list);
    return !list->size;
}

/**
 * @brief Get the first element of the list
 *
 * @param [in] list List
 * @param [out] iterator Iterator to the first element
 * @return Error Error code
 */
List::Error List::begin(List const* const list, ListIterator* const iterator) {
    assert(list);
    assert(iterator);

    iterator->ptr = list->bufferElem[0].next;

    return Error::OK;
}

/**
 * @brief Get the last element of the list
 *
 * @param [in] list List
 * @param [out] iterator Iterator to the last element
 * @return Error Error code
 */
List::Error List::rbegin(List const* const list, ListIterator* const iterator) {
    assert(list);
    assert(iterator);

    iterator->ptr = list->bufferElem[0].prev;

    return Error::OK;
}

/**
 * @brief Get list's outFunc
 *
 * @param [in] list List
 * @return ValueOutFunction_t outFunc
 */
ValueOutFunction_t List::getOutFunc(List const* const list) {
    assert(list);

    return list->outFunc;
}

/**
 * @brief Dump List info
 *
 * @param [in] list List
 * @return List::Error Error code
 */
List::Error List::dump(List* const list, LOGFUNCHEAD_ARGS_H) {
    assert(funcName);
    assert(fileName);

    Error err = Error::OK;

    logger::openLogFile();

    logger::logHtmlHead();
    logger__logFuncHead());
    if (list == nullptr) {
        logger::emergencyLog("List is nullptr");
        err = Error::NULLPTR_ERR;
    } else {

        logger::logStructHeadDebug("List", list, &list->debugInfo);

        logList(list);
    }
    logger::logHtmlTail();

    logger::closeLogFile();

    return err;
}
