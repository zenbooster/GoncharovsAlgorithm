#pragma once
#include <stdint.h>
#include <stdlib.h>

namespace GoncharovsAlgorithm {
class csGoncharovsAlgorithm
{
	static uint8_t tab8[256];
	static uint8_t tab16[65536];

	template <class T>
	static unsigned int _getNumberOfRowsFirst(T x, int& prevHi);
	template <class T>
	static unsigned int _getNumberOfRowsNext(T x, int& prevHi);

	template <class T>
	static int _initPrevHi(T x);
	template <class T>
	static int _initPrevLo(T x);
	template <class T>
	static int _getHi(T x);

public:
	csGoncharovsAlgorithm();

	static unsigned int getNumberOfRowsFirst(uint8_t x, int& prevLo);
	static unsigned int getNumberOfRowsNext(uint8_t x, int& prevLo);
	static unsigned int getNumberOfRowsFirst(uint16_t  x, int& prevLo);
	static unsigned int getNumberOfRowsNext(uint16_t x, int& prevLo);
	static unsigned int getNumberOfRowsFirst(uint32_t x, int& prevLo);
	static unsigned int getNumberOfRowsNext(uint32_t x, int& prevLo);
	static unsigned int getNumberOfRowsFirst(uint64_t x, int& prevLo);
	static unsigned int getNumberOfRowsNext(uint64_t x, int& prevLo);

	static unsigned int getNumberOfRows(uint8_t *p, size_t sz);
};
}
