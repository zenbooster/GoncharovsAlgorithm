#pragma once
#include "csGA.h"

namespace GA {
class csGATab: public csGA
{
	static uint8_t tab8[256];
	static uint8_t tab16[65536];

    public:
	    csGATab();
	    
		static inline unsigned int getNumberOfRowsFirst(uint8_t x, int& prevLo);
		static inline unsigned int getNumberOfRowsNext(uint8_t x, int& prevLo);
		static inline unsigned int getNumberOfRowsFirst(uint16_t x, int& prevLo);
		static inline unsigned int getNumberOfRowsNext(uint16_t x, int& prevLo);
		static inline unsigned int getNumberOfRowsFirst(uint32_t x, int& prevLo);
		static inline unsigned int getNumberOfRowsNext(uint32_t x, int& prevLo);
		static inline unsigned int getNumberOfRowsFirst(uint64_t x, int& prevLo);
		static inline unsigned int getNumberOfRowsNext(uint64_t x, int& prevLo);
	
		template<class T>
        static unsigned int getNumberOfRowsFirst(T *p, size_t sz, int& prevHi);
        
		template<class T>
        static unsigned int getNumberOfRowsNext(T *p, size_t sz, int& prevHi);
        
        template<class T>
		static unsigned int getNumberOfRows(T *p, size_t sz);
};

uint8_t csGATab::tab8[256];
uint8_t csGATab::tab16[65536];

csGATab::csGATab()
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

unsigned int csGATab::getNumberOfRowsFirst(uint8_t x, int& prevHi)
{
    prevHi = _initPrevHi(x);
    return getNumberOfRowsNext(x, prevHi);
}

unsigned int csGATab::getNumberOfRowsNext(uint8_t x, int& prevHi)
{
    unsigned int uiRes = csGATab::tab8[x];
    int lo = x & 1;
    if (lo == prevHi)
        uiRes--;

    prevHi = _getHi(x);

    return uiRes;
}

unsigned int csGATab::getNumberOfRowsFirst(uint16_t x, int& prevHi)
{
    prevHi = _initPrevHi(x);
    return getNumberOfRowsNext(x, prevHi);
}

unsigned int csGATab::getNumberOfRowsNext(uint16_t x, int& prevHi)
{
    unsigned int uiRes = csGATab::tab16[x];
    int lo = x & 1;
    if (lo == prevHi)
        uiRes--;

    prevHi = _getHi(x);

    return uiRes;
}

unsigned int csGATab::getNumberOfRowsFirst(uint32_t x, int& prevHi)
{
    prevHi = _initPrevHi(x);
    return getNumberOfRowsNext(x, prevHi);
}

unsigned int csGATab::getNumberOfRowsNext(uint32_t x, int& prevHi)
{
    int prev = prevHi;
    unsigned int uiRes = csGATab::getNumberOfRowsFirst((uint16_t)(x & 0xffff), prev);
    uiRes += csGATab::getNumberOfRowsNext((uint16_t)(x >> 16), prev);
    int lo = x & 1;
    if (lo == prevHi)
        uiRes--;

    prevHi = _getHi(x);

    return uiRes;
}

unsigned int csGATab::getNumberOfRowsFirst(uint64_t x, int& prevHi)
{
    prevHi = _initPrevHi(x);
    return getNumberOfRowsNext(x, prevHi);
}

unsigned int csGATab::getNumberOfRowsNext(uint64_t x, int& prevHi)
{
    int prev = prevHi;
    unsigned int uiRes = csGATab::getNumberOfRowsFirst((uint32_t)(x & 0xffffffff), prev);
    uiRes += csGATab::getNumberOfRowsNext((uint32_t)(x >> 32), prev);
    int lo = x & 1;
    if (lo == prevHi)
        uiRes--;

    prevHi = _getHi(x);

    return uiRes;
}

template<class T>
unsigned int csGATab::getNumberOfRowsFirst(T *p, size_t sz, int& prevHi)
{
	//cout << "unsigned int csGATab::getNumberOfRowsFirst(T *p, size_t sz, int& prevHi)" << endl;
    int res = _getNumberOfRows<csGATab, T>(
        p,
        sz,
        prevHi,
        true
    );
    return res;
}

template<class T>
unsigned int csGATab::getNumberOfRowsNext(T *p, size_t sz, int& prevHi)
{
    int res = _getNumberOfRows<csGATab, T>(
        p,
        sz,
        prevHi,
        false
    );
    return res;
}

template<class T>
unsigned int csGATab::getNumberOfRows(T *p, size_t sz)
{
	int prevHi;
	return getNumberOfRowsFirst(p, sz, prevHi);
}
}
