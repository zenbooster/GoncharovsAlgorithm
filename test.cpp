#include <sys/time.h>
#include <iostream>
#include "csGATab.h"

using namespace std;
using namespace GA;

int main(void)
{
	csGATab ga; // init tables...
	const size_t sz_buf = 1048576 * 10;
    uint8_t *buf = new uint8_t[sz_buf];
    struct timeval t;

    for(int i = 0; i < sz_buf; i++)
        //buf[i] = rand() & 0xff;
        buf[i] = 0x0f;

	cout << "Test of csPop:" << endl;
    gettimeofday(&t, NULL);
    uint64_t mt0 = (uint64_t)t.tv_sec * 1000 + t.tv_usec / 1000;
    int nr = csPop::pop((uint64_t*)buf, sz_buf);
    gettimeofday(&t, NULL);
    uint64_t mt1 = (uint64_t)t.tv_sec * 1000 + t.tv_usec / 1000;

    cout << "number of bits: " << nr << endl;
    cout << "execution time: " << mt1-mt0 << " uS" << endl;
    cout << endl;

	cout << "Test without tables:" << endl;
    gettimeofday(&t, NULL);
    mt0 = (uint64_t)t.tv_sec * 1000 + t.tv_usec / 1000;
    nr = csGA::getNumberOfRows((uint64_t*)buf, sz_buf);
    gettimeofday(&t, NULL);
    mt1 = (uint64_t)t.tv_sec * 1000 + t.tv_usec / 1000;

    cout << "number of bit runs: " << nr << endl;
    cout << "execution time: " << mt1-mt0 << " uS" << endl;
    cout << endl;

	cout << "Test with tables:" << endl;
    gettimeofday(&t, NULL);
    mt0 = (uint64_t)t.tv_sec * 1000 + t.tv_usec / 1000;
    nr = csGATab::getNumberOfRows((uint64_t*)buf, sz_buf);
    gettimeofday(&t, NULL);
    delete [] buf;
	mt1 = (uint64_t)t.tv_sec * 1000 + t.tv_usec / 1000;

    cout << "number of bit runs: " << nr << endl;
    cout << "execution time: " << mt1-mt0 << " uS" << endl;

    return 0;
}
