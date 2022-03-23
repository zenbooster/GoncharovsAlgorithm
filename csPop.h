#pragma once
#include <stdint.h>

namespace GoncharovsAlgorithm
{
class csPop
{
    private:
        static constexpr uint64_t m1 = 0x5555555555555555; //binary: 0101...
        static constexpr uint64_t m2 = 0x3333333333333333; //binary: 00110011..
        static constexpr uint64_t m4 = 0x0f0f0f0f0f0f0f0f; //binary:  4 zeros,  4 ones ...

        template <class T>
        static T pop8(T x)
        {
            x -= (x >> 1) & m1;             //put count of each 2 bits into those 2 bits
            x = (x & m2) + ((x >> 2) & m2); //put count of each 4 bits into those 4 bits 
            x = (x + (x >> 4)) & m4;        //put count of each 8 bits into those 8 bits
            return x;
        }

        template <class T>
        static T pop16(T x)
        {
            x = pop8(x);
            x += x >> 8;  //put count of each 16 bits into their lowest 8 bits
            return x;
        }

        template <class T>
        static T pop32(T x)
        {
            x = pop16(x);
            x += x >> 16;  //put count of each 32 bits into their lowest 8 bits
            return x;
        }

        template <class T>
        static T pop64(T x)
        {
            x = pop32(x);
            x += x >> 32;  //put count of each 64 bits into their lowest 8 bits
            return x;
        }

    public:
        static int pop(uint8_t x)
        {
            x = pop8(x);
            return x & 0x7f;
        }

        static int pop(uint16_t x)
        {
            x = pop16(x);
            return x & 0x7f;
        }

        static int pop(uint32_t x)
        {
            x = pop32(x);
            return x & 0x7f;
        }

        static int pop(uint64_t x)
        {
            x = pop64(x);
            return x & 0x7f;
        }

        static int pop(uint8_t *p, size_t sz)
        {
            int res = 0;
            uint64_t *p64 = (uint64_t *)p;
            uint64_t *p64end = (uint64_t *)(p + sz);
            for(;p64 < p64end; p64++)
            {
                res += pop(*p64);
            }
            return res;
        }
};
}
