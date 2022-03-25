#pragma once
#include <stdint.h>
#include <stdlib.h>
#include "csPop.h"

namespace GoncharovsAlgorithm {
class csGoncharovsAlgorithm
{
protected:
	template <class T>
	static inline int _initPrevHi(T x);
	template <class T>
	static inline int _getHi(T x);

	template <class T>
	static inline unsigned int _getNumberOfRowsFirst(T x, int& prevHi);
	template <class T>
	static inline unsigned int _getNumberOfRowsNext(T x, int& prevHi);

	template<typename T>
	static unsigned int _getNumberOfRows(T *p, size_t sz, unsigned int(*ff)(T, int&), unsigned int(*fn)(T, int&));

public:
	template<class T>
	static unsigned int getNumberOfRows(T *p, size_t sz);
};

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

template<class T>
unsigned int csGoncharovsAlgorithm::_getNumberOfRows(T *p, size_t sz, unsigned int(*ff)(T, int&), unsigned int(*fn)(T, int&))
{
    int res;
    T *pend = (T*)((uint8_t*)p + sz);
    int prevHi;
    
    res = ff(*p++, prevHi);
    for(;p < pend; p++)
    {
        res += fn(*p, prevHi);
    }
    return res;
}

template<class T>
unsigned int csGoncharovsAlgorithm::getNumberOfRows(T *p, size_t sz)
{
    int res = _getNumberOfRows<T>(
        p,
        sz,
        _getNumberOfRowsFirst,
        _getNumberOfRowsNext
    );
    return res;
}
}
