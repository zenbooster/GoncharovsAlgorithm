#include <sys/time.h>
#include <iostream>
#include "csGoncharovsAlgorithm.h"

using namespace std;
using namespace GoncharovsAlgorithm;

csGoncharovsAlgorithm ga; // init tables...

int main(void)
{
    uint8_t buf[1048576];
    struct timeval t;

    for(int i = 0; i < sizeof(buf); i++)
        buf[i] = rand() & 0xff;

    gettimeofday(&t, NULL);
    uint64_t mt0 = (uint64_t)t.tv_sec * 1000 + t.tv_usec / 1000;
    int r = csGoncharovsAlgorithm::getNumberOfRows(buf, sizeof(buf));
    gettimeofday(&t, NULL);
    uint64_t mt1 = (uint64_t)t.tv_sec * 1000 + t.tv_usec / 1000;

    cout << "r = " << r << endl;
    cout << "mt = " << mt1-mt0 << endl;

    return 0;
}
