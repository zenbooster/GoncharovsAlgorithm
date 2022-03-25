#pragma once
#include "csGoncharovsAlgorithm.h"

namespace GoncharovsAlgorithm {

class csGoncharovsAlgorithmTab: public csGoncharovsAlgorithm
{
	static uint8_t tab8[256];
	static uint8_t tab16[65536];

    public:
	    csGoncharovsAlgorithmTab();
	    
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

uint8_t csGoncharovsAlgorithmTab::tab8[256];
uint8_t csGoncharovsAlgorithmTab::tab16[65536];

csGoncharovsAlgorithmTab::csGoncharovsAlgorithmTab()
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

unsigned int csGoncharovsAlgorithmTab::getNumberOfRowsFirst(uint8_t x, int& prevHi)
{
    prevHi = _initPrevHi(x);
    return getNumberOfRowsNext(x, prevHi);
}

unsigned int csGoncharovsAlgorithmTab::getNumberOfRowsNext(uint8_t x, int& prevHi)
{
    unsigned int uiRes = csGoncharovsAlgorithmTab::tab8[x];
    int lo = x & 1;
    if (lo == prevHi)
        uiRes--;

    prevHi = _getHi(x);

    return uiRes;
}

unsigned int csGoncharovsAlgorithmTab::getNumberOfRowsFirst(uint16_t x, int& prevHi)
{
    prevHi = _initPrevHi(x);
    return getNumberOfRowsNext(x, prevHi);
}

unsigned int csGoncharovsAlgorithmTab::getNumberOfRowsNext(uint16_t x, int& prevHi)
{
    unsigned int uiRes = csGoncharovsAlgorithmTab::tab16[x];
    int lo = x & 1;
    if (lo == prevHi)
        uiRes--;

    prevHi = _getHi(x);

    return uiRes;
}

unsigned int csGoncharovsAlgorithmTab::getNumberOfRowsFirst(uint32_t x, int& prevHi)
{
    prevHi = _initPrevHi(x);
    return getNumberOfRowsNext(x, prevHi);
}

unsigned int csGoncharovsAlgorithmTab::getNumberOfRowsNext(uint32_t x, int& prevHi)
{
    int prev = prevHi;
    unsigned int uiRes = csGoncharovsAlgorithmTab::getNumberOfRowsFirst((uint16_t)(x & 0xffff), prev);
    uiRes += csGoncharovsAlgorithmTab::getNumberOfRowsNext((uint16_t)(x >> 16), prev);
    int lo = x & 1;
    if (lo == prevHi)
        uiRes--;

    prevHi = _getHi(x);

    return uiRes;
}

unsigned int csGoncharovsAlgorithmTab::getNumberOfRowsFirst(uint64_t x, int& prevHi)
{
    prevHi = _initPrevHi(x);
    return getNumberOfRowsNext(x, prevHi);
}

unsigned int csGoncharovsAlgorithmTab::getNumberOfRowsNext(uint64_t x, int& prevHi)
{
    int prev = prevHi;
    unsigned int uiRes = csGoncharovsAlgorithmTab::getNumberOfRowsFirst((uint32_t)(x & 0xffffffff), prev);
    uiRes += csGoncharovsAlgorithmTab::getNumberOfRowsNext((uint32_t)(x >> 32), prev);
    int lo = x & 1;
    if (lo == prevHi)
        uiRes--;

    prevHi = _getHi(x);

    return uiRes;
}

unsigned int csGoncharovsAlgorithmTab::getNumberOfRows(uint8_t *p, size_t sz)
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
