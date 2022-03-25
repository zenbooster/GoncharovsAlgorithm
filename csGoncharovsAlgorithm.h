#pragma once
#include <stdint.h>
#include <stdlib.h>
#include "csPop.h"

namespace GoncharovsAlgorithm {
class csGoncharovsAlgorithm
{
	static uint8_t tab8[256];
	static uint8_t tab16[65536];

	template <class T>
	static inline int _initPrevHi(T x);
	template <class T>
	static inline int _getHi(T x);

protected:
	template <class T>
	static inline unsigned int _getNumberOfRowsFirst(T x, int& prevHi);
	template <class T>
	static inline unsigned int _getNumberOfRowsNext(T x, int& prevHi);

	template<typename T>
	static unsigned int _getNumberOfRows(uint8_t *p, size_t sz, unsigned int(*ff)(T, int&), unsigned int(*fn)(T, int&));

public:
	csGoncharovsAlgorithm();

	static inline unsigned int getNumberOfRowsFirst(uint8_t x, int& prevLo);
	static inline unsigned int getNumberOfRowsNext(uint8_t x, int& prevLo);
	static inline unsigned int getNumberOfRowsFirst(uint16_t x, int& prevLo);
	static inline unsigned int getNumberOfRowsNext(uint16_t x, int& prevLo);
	static inline unsigned int getNumberOfRowsFirst(uint32_t x, int& prevLo);
	static inline unsigned int getNumberOfRowsNext(uint32_t x, int& prevLo);
	static inline unsigned int getNumberOfRowsFirst(uint64_t x, int& prevLo);
	static inline unsigned int getNumberOfRowsNext(uint64_t x, int& prevLo);

	static unsigned int getNumberOfRows(uint8_t *p, size_t sz);
};

uint8_t csGoncharovsAlgorithm::tab8[256];
uint8_t csGoncharovsAlgorithm::tab16[65536];

// Подсчитывает количество рядов в первом элементе.
// Параметр prevHi возвращает состояние старшего бита в целях последующего объединения рядов, идущих через границу между элементами.
template <class T>
unsigned int csGoncharovsAlgorithm::_getNumberOfRowsFirst(T x, int& prevHi)
{
    prevHi = _initPrevHi(x);
    return _getNumberOfRowsNext(x, prevHi);
}

// Подсчитывает количество рядов в следующем элементе.
// Параметр prevHi используется для объединения рядов, идущих через границу между элементами.
template <class T>
unsigned int csGoncharovsAlgorithm::_getNumberOfRowsNext(T x, int& prevHi)
{
    unsigned int uiRes = csPop::pop(static_cast<T>(x ^ ((x << 1) | prevHi)));
    prevHi = _getHi(x);

    return uiRes;
}

template <class T>
int csGoncharovsAlgorithm::_initPrevHi(T x)
{
    return 1 - (x & 1);
}

template <class T>
int csGoncharovsAlgorithm::_getHi(T x)
{
    return x >> ((sizeof(x) << 3) - 1);
}

csGoncharovsAlgorithm::csGoncharovsAlgorithm()
{
    for (int prevHi, i = 0; i < 0x100; i++)
    {
        tab8[i] = _getNumberOfRowsFirst((uint8_t)i, prevHi);
    }

    for (int prevHi, i = 0; i < 0x10000; i++)
    {
        uint8_t x = getNumberOfRowsFirst((uint8_t)(i & 0xff), prevHi);
        x += getNumberOfRowsNext((uint8_t)(i >> 8), prevHi);
        tab16[i] = x;
    }
}

unsigned int csGoncharovsAlgorithm::getNumberOfRowsFirst(uint8_t x, int& prevHi)
{
    prevHi = _initPrevHi(x);
    return getNumberOfRowsNext(x, prevHi);
}

unsigned int csGoncharovsAlgorithm::getNumberOfRowsNext(uint8_t x, int& prevHi)
{
    unsigned int uiRes = csGoncharovsAlgorithm::tab8[x];
    int lo = x & 1;
    if (lo == prevHi)
        uiRes--;

    prevHi = _getHi(x);

    return uiRes;
}

unsigned int csGoncharovsAlgorithm::getNumberOfRowsFirst(uint16_t x, int& prevHi)
{
    prevHi = _initPrevHi(x);
    return getNumberOfRowsNext(x, prevHi);
}

unsigned int csGoncharovsAlgorithm::getNumberOfRowsNext(uint16_t x, int& prevHi)
{
    unsigned int uiRes = csGoncharovsAlgorithm::tab16[x];
    int lo = x & 1;
    if (lo == prevHi)
        uiRes--;

    prevHi = _getHi(x);

    return uiRes;
}

unsigned int csGoncharovsAlgorithm::getNumberOfRowsFirst(uint32_t x, int& prevHi)
{
    prevHi = _initPrevHi(x);
    return getNumberOfRowsNext(x, prevHi);
}

unsigned int csGoncharovsAlgorithm::getNumberOfRowsNext(uint32_t x, int& prevHi)
{
    int prev = prevHi;
    unsigned int uiRes = csGoncharovsAlgorithm::getNumberOfRowsFirst((uint16_t)(x & 0xffff), prev);
    uiRes += csGoncharovsAlgorithm::getNumberOfRowsNext((uint16_t)(x >> 16), prev);
    int lo = x & 1;
    if (lo == prevHi)
        uiRes--;

    prevHi = _getHi(x);

    return uiRes;
}

unsigned int csGoncharovsAlgorithm::getNumberOfRowsFirst(uint64_t x, int& prevHi)
{
    prevHi = _initPrevHi(x);
    return getNumberOfRowsNext(x, prevHi);
}

unsigned int csGoncharovsAlgorithm::getNumberOfRowsNext(uint64_t x, int& prevHi)
{
    int prev = prevHi;
    unsigned int uiRes = csGoncharovsAlgorithm::getNumberOfRowsFirst((uint32_t)(x & 0xffffffff), prev);
    uiRes += csGoncharovsAlgorithm::getNumberOfRowsNext((uint32_t)(x >> 32), prev);
    int lo = x & 1;
    if (lo == prevHi)
        uiRes--;

    prevHi = _getHi(x);

    return uiRes;
}

template<typename T>
unsigned int csGoncharovsAlgorithm::_getNumberOfRows(uint8_t *p, size_t sz, unsigned int(*ff)(T, int&), unsigned int(*fn)(T, int&))
{
    int res;
    T *pcur = (T *)p;
    T *pend = (T *)(p + sz);
    int prevHi;
    
    res = ff(*pcur++, prevHi);
    for(;pcur < pend; pcur++)
    {
        res += fn(*pcur, prevHi);
    }
    return res;
}

unsigned int csGoncharovsAlgorithm::getNumberOfRows(uint8_t *p, size_t sz)
{
    int res = _getNumberOfRows<uint64_t>(
        p,
        sz,
        getNumberOfRowsFirst,
        getNumberOfRowsNext
    );
    return res;
}
}
