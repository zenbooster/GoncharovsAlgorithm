#pragma once
#include <iostream>
#include "csPop.h"

using namespace std;

namespace GA {
class csGA
{
private:
	template<class T>
	static inline bool _chkIfTAvail(T *p, T *pend);
	
protected:
	template <class T>
	static inline int _initPrevHi(T x);
	template <class T>
	static inline int _getHi(T x);

	template <class T>
	static inline unsigned int _getNumberOfRowsFirst(T x, int& prevHi);
	template <class T>
	static inline unsigned int _getNumberOfRowsNext(T x, int& prevHi);
	template <class C, class T>
	static unsigned int _getNumberOfRows(T x, int& prevHi, bool& isFirst);

	template <class C, class T>
	static unsigned int _getNumberOfRows(T *p, size_t sz, int& prevHi, bool isFirst);

public:
	template<class T>
	static unsigned int getNumberOfRowsFirst(T *p, size_t sz, int& prevHi);
	
	template<class T>
	static unsigned int getNumberOfRowsNext(T *p, size_t sz, int& prevHi);

	template<class T>
	static unsigned int getNumberOfRows(T *p, size_t sz);
};

// Подсчитывает количество рядов в первом элементе.
// Параметр prevHi возвращает состояние старшего бита в целях последующего объединения рядов, идущих через границу между элементами.
template <class T>
unsigned int csGA::_getNumberOfRowsFirst(T x, int& prevHi)
{
    prevHi = _initPrevHi(x);
    return _getNumberOfRowsNext(x, prevHi);
}

// Подсчитывает количество рядов в следующем элементе.
// Параметр prevHi используется для объединения рядов, идущих через границу между элементами.
template <class T>
unsigned int csGA::_getNumberOfRowsNext(T x, int& prevHi)
{
    unsigned int uiRes = csPop::pop(static_cast<T>(x ^ ((x << 1) | prevHi)));
    prevHi = _getHi(x);

    return uiRes;
}

template <class T>
int csGA::_initPrevHi(T x)
{
    return 1 - (x & 1);
}

template <class T>
int csGA::_getHi(T x)
{
    return x >> ((sizeof(x) << 3) - 1);
}

template <class C, class T>
unsigned int csGA::_getNumberOfRows(T x, int& prevHi, bool& isFirst)
{
	if (isFirst)
	{
    	prevHi = _initPrevHi(x);
    	isFirst = false;
    }

    return C::_getNumberOfRowsNext(x, prevHi);
}

template<class T>
bool csGA::_chkIfTAvail(T *p, T *pend)
{
	return ((uint8_t*)pend-(uint8_t*)p) >= sizeof(T);
}

template<class C, class T>
unsigned int csGA::_getNumberOfRows(T *p, size_t sz, int& prevHi, bool isFirst)
{
    int res = 0;
    T *pend = (T*)((uint8_t*)p + sz);
   
    if (sz >= sizeof(T))
    {
    	int t = (size_t)p & (sizeof(T) - 1);
    	
    	sz -= t;

    	if (t >= sizeof(uint8_t))
    	{
    		res += _getNumberOfRows<C>(*(uint8_t*)p, prevHi, isFirst);
    		p = (T*)((uint8_t*)p + 1);
    		t -= sizeof(uint8_t);
    	}    	
    	if (t >= sizeof(uint16_t))
    	{
    		res += _getNumberOfRows<C>(*(uint16_t*)p, prevHi, isFirst);
    		p = (T*)((uint16_t*)p + 1);
    		t -= sizeof(uint16_t);
    	}    	
    	if (t >= sizeof(uint32_t))
    	{
    		res += _getNumberOfRows<C>(*(uint32_t*)p, prevHi, isFirst);
    		p = (T*)((uint32_t*)p + 1);
    		t -= sizeof(uint32_t);
    	}

		if (_chkIfTAvail(p, pend))
    	{
    		sz -= sizeof(T);
			res += _getNumberOfRows<C>(*p++, prevHi, isFirst);

			for (; _chkIfTAvail(p, pend); p++, sz-= sizeof(T))
    		{
        		res += C::_getNumberOfRowsNext(*p, prevHi);
    		}
    	}
    } // if (sz >= sizeof(T))

    if (sz >= sizeof(uint32_t))
    {
    	res += _getNumberOfRows<C>(*(uint32_t*)p, prevHi, isFirst);
    	p = (T*)((uint32_t*)p + 1);
    	sz -= sizeof(uint32_t);
    }
    if (sz >= sizeof(uint16_t))
    {
    	res += _getNumberOfRows<C>(*(uint16_t*)p, prevHi, isFirst);
    	p = (T*)((uint16_t*)p + 1);
    	sz -= sizeof(uint16_t);
    }
    if (sz >= sizeof(uint8_t))
    {
    	res += _getNumberOfRows<C>(*(uint8_t*)p, prevHi, isFirst);
    	p = (T*)((uint8_t*)p + 1);
    	sz -= sizeof(uint8_t);
    }

    return res;
}

template<class T>
unsigned int csGA::getNumberOfRowsFirst(T *p, size_t sz, int& prevHi)
{
	cout << "unsigned int csGA::getNumberOfRowsFirst(T *p, size_t sz, int& prevHi)" << endl;
    int res = _getNumberOfRows<csGA, T>(
        p,
        sz,
        prevHi,
        true
    );
    return res;
}

template<class T>
unsigned int csGA::getNumberOfRowsNext(T *p, size_t sz, int& prevHi)
{
    int res = _getNumberOfRows<csGA, T>(
        p,
        sz,
        prevHi,
        false
    );
    return res;
}

template<class T>
unsigned int csGA::getNumberOfRows(T *p, size_t sz)
{
	int prevHi;
	return getNumberOfRowsFirst(p, sz, prevHi);
}
}
