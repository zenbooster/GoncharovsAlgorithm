#include <sys/time.h>
#include <iostream>

#include "csGATab.h"

using namespace std;
using namespace GA;

typedef unsigned __int128 uint128_t;

//typedef uint256_t TT;
//typedef uint128_t TT;
typedef uint64_t TT;
//typedef uint32_t TT;

int main(void)
{
	//cout << sizeof(TT) << endl;
	//TT v = stBitPattern<TT>::res(0x0f);
	//cout << csPop::pop(v) << endl;
	csGATab ga; // init tables...
	const size_t sz_buf = 1048576 * 10;
	//const size_t sz_buf = 256;
	int prevHi;
    uint8_t *buf = new uint8_t[sz_buf];
    struct timeval t;

    for(int i = 0; i < sz_buf; i++)
        buf[i] = 0x0f;

	cout << "Test of csPop:" << endl;
    gettimeofday(&t, NULL);
    uint64_t mt0 = (uint64_t)t.tv_sec * 1000 + t.tv_usec / 1000;
    int nr = csPop::pop((TT*)buf, sz_buf);
    gettimeofday(&t, NULL);
    uint64_t mt1 = (uint64_t)t.tv_sec * 1000 + t.tv_usec / 1000;

    cout << "number of bits: " << nr << endl;
    cout << "execution time: " << mt1-mt0 << " uS" << endl;
    cout << endl;

	cout << "Test without tables:" << endl;
    gettimeofday(&t, NULL);
    mt0 = (uint64_t)t.tv_sec * 1000 + t.tv_usec / 1000;
    nr = csGA::getNumberOfRows((TT*)buf, sz_buf);
    gettimeofday(&t, NULL);
    mt1 = (uint64_t)t.tv_sec * 1000 + t.tv_usec / 1000;

    cout << "number of bit runs: " << nr << endl;
    cout << "execution time: " << mt1-mt0 << " uS" << endl;
    cout << endl;

	cout << "Test with tables:" << endl;
    gettimeofday(&t, NULL);
    mt0 = (uint64_t)t.tv_sec * 1000 + t.tv_usec / 1000;
    nr = csGATab::getNumberOfRows((TT*)buf, sz_buf);
    gettimeofday(&t, NULL);

    delete [] buf;
	mt1 = (uint64_t)t.tv_sec * 1000 + t.tv_usec / 1000;

    cout << "number of bit runs: " << nr << endl;
    cout << "execution time: " << mt1-mt0 << " uS" << endl;

    return 0;
}
