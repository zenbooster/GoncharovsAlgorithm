#include <sys/time.h>
#include <functional>
#include <iostream>
#include "csGATab.h"

using namespace std;
using namespace GA;

typedef unsigned __int128 uint128_t;

class csTestSpeed
{
public:
	template <class T, int cnt>
	static void run(string title, string title_result, std::function<T(void)> f)
	{
		struct timeval t;
		double avg_us = 0;
		T r;

		cout << title << ":" << endl;
		for(int i = 0; i < cnt; i++)
		{
    		gettimeofday(&t, NULL);
    		uint64_t mt0 = (uint64_t)t.tv_sec * 1000 + t.tv_usec / 1000;
    		r = f();
    		gettimeofday(&t, NULL);
    		uint64_t mt1 = (uint64_t)t.tv_sec * 1000 + t.tv_usec / 1000;
    		uint64_t us = mt1 - mt0;
    		
    		int n = i + 1;
    		avg_us += (us - avg_us) / n;
    	}
    	cout << title_result << ": " << r << "" << endl;
	   	cout << "avg execution time: " << avg_us << " uS" << endl;
	}
};

template <class T, int epochs>
void TestSpeed(T *buf, const size_t sz_buf)
{
	cout << ">>>> Testing for elements of size " << (sizeof(T) << 3) << " bits <<<<" << endl;
	csTestSpeed::run<int, epochs>("Test of csPop", "number of bits", [buf, sz_buf](){return csPop::pop(buf, sz_buf);});
	csTestSpeed::run<int, epochs>("Test without tables", "number of bit runs", [buf, sz_buf](){return csGA::getNumberOfRows(buf, sz_buf);});
	csTestSpeed::run<int, epochs>("Test with tables", "number of bit runs", [buf, sz_buf](){return csGATab::getNumberOfRows(buf, sz_buf);});
}

int main(void)
{
	constexpr int epochs = 100;
	csGATab ga; // init tables...
	const size_t sz_buf = 1048576; // 1M
	int prevHi;
    uint8_t *buf = new uint8_t[sz_buf];
    struct timeval t;
    
    cout << "Number of epochs: " << epochs << endl;
    cout << "Buffer size: " << sz_buf << " bytes" << endl;
    cout << endl;

    for(int i = 0; i < sz_buf; i++)
        buf[i] = 0x0f;

	TestSpeed<uint8_t, epochs>(buf, sz_buf);
	cout << endl;
	TestSpeed<uint16_t, epochs>((uint16_t*)buf, sz_buf);
	cout << endl;
	TestSpeed<uint32_t, epochs>((uint32_t*)buf, sz_buf);
	cout << endl;
	TestSpeed<uint64_t, epochs>((uint64_t*)buf, sz_buf);
	cout << endl;
	TestSpeed<uint128_t, epochs>((uint128_t*)buf, sz_buf);

    delete [] buf;

    return 0;
}
