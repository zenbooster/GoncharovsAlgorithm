#pragma once
#include "stBitPattern.h"

namespace GA
{
class csPop
{
    private:
        template <class T>
        static inline constexpr T _pop8(T x)
        {
        	constexpr T m1 = stBitPattern<T>::res(0x55); //binary: 0101...
	        constexpr T m2 = stBitPattern<T>::res(0x33); //binary: 00110011..
    	    constexpr T m4 = stBitPattern<T>::res(0x0f); //binary:  4 zeros,  4 ones ...

            x -= (x >> 1) & m1;             //put count of each 2 bits into those 2 bits
            x = (x & m2) + ((x >> 2) & m2); //put count of each 4 bits into those 4 bits 
            x = (x + (x >> 4)) & m4;        //put count of each 8 bits into those 8 bits
            return x;
        }

        template <class T, int width = sizeof(T) << 3>
        struct _pop
        {
        	static inline constexpr T res(T x)
        	{
        		const int hw = width >> 1;
	
	        	x = _pop<T, hw>::res(x);
	        	x += x >> hw;
	
            	return x;
           	}
        };

        template <class T>
        struct _pop<T, 8>
        {
	        static inline constexpr T res(T x)
	        {
	       		x = _pop8(x);
	            return x;
	        }
        };

		template<class T>
		static inline constexpr bool _chkIfTAvail(T *p, T *pend);

    public:
		template <class T>
        static inline constexpr int pop(T x)
        {
            x = _pop<T>::res(x);
            return (int)(x & 0x7f);
        }

		template <class T>
        static int pop(T *p, size_t sz);
};

template<class T>
constexpr bool csPop::_chkIfTAvail(T *p, T *pend)
{
	return ((uint8_t*)pend-(uint8_t*)p) >= sizeof(T);
}

template <class T>
int csPop::pop(T *p, size_t sz)
{
    int res = 0;
    T *pend = (T*)((uint8_t*)p + sz);
    
    if (sz >= sizeof(T))
    {
    	int t = (size_t)p & (sizeof(T) - 1);
    	
    	sz -= t;

    	if (t >= sizeof(uint8_t))
    	{
    		res += pop(*(uint8_t*)p);
    		p = (T*)((uint8_t*)p + 1);
    		t -= sizeof(uint8_t);
    	}    	
    	if (t >= sizeof(uint16_t))
    	{
    		res += pop(*(uint16_t*)p);
    		p = (T*)((uint16_t*)p + 1);
    		t -= sizeof(uint16_t);
    	}    	
    	if (t >= sizeof(uint32_t))
    	{
    		res += pop(*(uint32_t*)p);
    		p = (T*)((uint32_t*)p + 1);
    		t -= sizeof(uint32_t);
    	}
    	if (t >= sizeof(uint64_t))
    	{
    		res += pop(*(uint64_t*)p);
    		p = (T*)((uint64_t*)p + 1);
    		t -= sizeof(uint64_t);
    	}

		if (_chkIfTAvail(p, pend))
    	{
    		sz -= sizeof(T);
			res += pop(*p++);

    		for(; _chkIfTAvail(p, pend); p++, sz-= sizeof(T))
    		{
        		res += pop(*p);
    		}		
    	}
    } // if (sz >= sizeof(T))

    if (sz >= sizeof(uint64_t))
    {
    	res += pop(*(uint64_t*)p);
    	p = (T*)((uint64_t*)p + 1);
    	sz -= sizeof(uint64_t);
    }
    if (sz >= sizeof(uint32_t))
    {
    	res += pop(*(uint32_t*)p);
    	p = (T*)((uint32_t*)p + 1);
    	sz -= sizeof(uint32_t);
    }
    if (sz >= sizeof(uint16_t))
    {
    	res += pop(*(uint16_t*)p);
    	p = (T*)((uint16_t*)p + 1);
    	sz -= sizeof(uint16_t);
    }
    if (sz >= sizeof(uint8_t))
    {
    	res += pop(*(uint8_t*)p);
    	p = (T*)((uint8_t*)p + 1);
    	sz -= sizeof(uint8_t);
    }

    return res;
}
}
