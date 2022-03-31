#pragma once
#include <stdint.h>

template <class T, int width = sizeof(T) << 3>
struct stBitPattern
{
    static inline constexpr T res(T x)
    {
    	constexpr int hw = width >> 1;

        x = stBitPattern<T, hw>::res(x);
        x += x << hw;

        return x;
    }
};

template <class T>
struct stBitPattern<T, 8>
{
    static inline constexpr T res(T x)
    {
        return x;
    }
};
