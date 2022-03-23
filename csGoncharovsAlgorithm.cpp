#include "csGoncharovsAlgorithm.h"
#include "csPop.h"

using namespace GoncharovsAlgorithm;

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
int csGoncharovsAlgorithm::_initPrevLo(T x)
{
    return 1 - (x >> ((sizeof(x) << 3) - 1));
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

    for (int prevLo, i = 0; i < 0x10000; i++)
    {
        uint8_t x = getNumberOfRowsFirst((uint8_t)(i & 0xff), prevLo);
        x += getNumberOfRowsNext((uint8_t)(i >> 8), prevLo);
        tab16[i] = x;
    }
}

unsigned int csGoncharovsAlgorithm::getNumberOfRowsFirst(uint8_t x, int& prevLo)
{
    prevLo = _initPrevLo(x);
    return getNumberOfRowsNext(x, prevLo);
}

unsigned int csGoncharovsAlgorithm::getNumberOfRowsNext(uint8_t x, int& prevLo)
{
    unsigned int uiRes = csGoncharovsAlgorithm::tab8[x];
    int hi = _getHi(x);
    if (hi == prevLo)
        uiRes--;

    prevLo = x & 1;

    return uiRes;
}

unsigned int csGoncharovsAlgorithm::getNumberOfRowsFirst(uint16_t x, int& prevLo)
{
    prevLo = _initPrevLo(x);
    return getNumberOfRowsNext(x, prevLo);
}

unsigned int csGoncharovsAlgorithm::getNumberOfRowsNext(uint16_t x, int& prevLo)
{
    unsigned int uiRes = csGoncharovsAlgorithm::tab16[x];
    int hi = _getHi(x);
    if (hi == prevLo)
        uiRes--;

    prevLo = x & 1;

    return uiRes;
}

unsigned int csGoncharovsAlgorithm::getNumberOfRowsFirst(uint32_t x, int& prevLo)
{
    prevLo = _initPrevLo(x);
    return getNumberOfRowsNext(x, prevLo);
}

unsigned int csGoncharovsAlgorithm::getNumberOfRowsNext(uint32_t x, int& prevLo)
{
    unsigned int uiRes = csGoncharovsAlgorithm::getNumberOfRowsFirst((uint16_t)(x & 0xffff), prevLo);
    uiRes += csGoncharovsAlgorithm::getNumberOfRowsNext((uint16_t)(x >> 16), prevLo);
    int hi = _getHi(x);
    if (hi == prevLo)
        uiRes--;

    prevLo = x & 1;

    return uiRes;
}

unsigned int csGoncharovsAlgorithm::getNumberOfRowsFirst(uint64_t x, int& prevLo)
{
    prevLo = _initPrevLo(x);
    return getNumberOfRowsNext(x, prevLo);
}

unsigned int csGoncharovsAlgorithm::getNumberOfRowsNext(uint64_t x, int& prevLo)
{
    unsigned int uiRes = csGoncharovsAlgorithm::getNumberOfRowsFirst((uint32_t)(x & 0xffffffff), prevLo);
    uiRes += csGoncharovsAlgorithm::getNumberOfRowsNext((uint32_t)(x >> 32), prevLo);
    int hi = _getHi(x);
    if (hi == prevLo)
        uiRes--;

    prevLo = x & 1;

    return uiRes;
}

unsigned int csGoncharovsAlgorithm::getNumberOfRows(uint8_t *p, size_t sz)
{
    int res;
    uint64_t *p64 = (uint64_t *)p;
    uint64_t *p64end = (uint64_t *)(p + sz);
    int prevLo;
    
    res = getNumberOfRowsFirst(*p64++, prevLo);
    for(;p64 < p64end; p64++)
    {
        res += getNumberOfRowsNext(*p64, prevLo);
    }
    return res;
}
