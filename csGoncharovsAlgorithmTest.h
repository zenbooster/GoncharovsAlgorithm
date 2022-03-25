#pragma once
#include "csGoncharovsAlgorithm.h"

namespace GoncharovsAlgorithm {

class csGoncharovsAlgorithmTest: public csGoncharovsAlgorithm
{
    public:
        static unsigned int getNumberOfRowsNoTab(uint8_t *p, size_t sz);
};

unsigned int csGoncharovsAlgorithmTest::getNumberOfRowsNoTab(uint8_t *p, size_t sz)
{
    int res = _getNumberOfRows<uint64_t>(
        p,
        sz,
        _getNumberOfRowsFirst,
        _getNumberOfRowsNext
    );
    return res;
}
}
