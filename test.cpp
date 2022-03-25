#include <sys/time.h>
#include <iostream>
#include "csGoncharovsAlgorithmTest.h"

using namespace std;
using namespace GoncharovsAlgorithm;

csGoncharovsAlgorithm ga; // init tables...

int main(void)
{
	const size_t sz_buf = 1048576 * 10;
    uint8_t *buf = new uint8_t[sz_buf];
    struct timeval t;

    for(int i = 0; i < sz_buf; i++)
        buf[i] = rand() & 0xff;

	cout << "Test without tables:" << endl;
    gettimeofday(&t, NULL);
    uint64_t mt0 = (uint64_t)t.tv_sec * 1000 + t.tv_usec / 1000;
    int nr = csGoncharovsAlgorithmTest::getNumberOfRowsNoTab(buf, sz_buf);
    gettimeofday(&t, NULL);
    uint64_t mt1 = (uint64_t)t.tv_sec * 1000 + t.tv_usec / 1000;

    cout << "number of bit runs: " << nr << endl;
    cout << "execution time: " << mt1-mt0 << " uS" << endl;
    cout << endl;

	cout << "Test with tables:" << endl;
    gettimeofday(&t, NULL);
    mt0 = (uint64_t)t.tv_sec * 1000 + t.tv_usec / 1000;
    nr = csGoncharovsAlgorithmTest::getNumberOfRows(buf, sz_buf);
    gettimeofday(&t, NULL);
    delete [] buf;
	mt1 = (uint64_t)t.tv_sec * 1000 + t.tv_usec / 1000;

    cout << "number of bit runs: " << nr << endl;
    cout << "execution time: " << mt1-mt0 << " uS" << endl;

    return 0;
}