//
// CAutoTicker.h
//

#ifndef __H_CAutoTicker__
#define __H_CAutoTicker__

#include <chrono>
#include <iostream>

class CAutoTicker
{
public:
	CAutoTicker(const char* tag) {
		mStartTime = std::chrono::high_resolution_clock::now();
		mTag = tag;
	}

	~CAutoTicker() {
		std::chrono::duration<double, std::milli> elapsed = std::chrono::high_resolution_clock::now() - mStartTime;
		std::cout << "[" << mTag << "] it takes " << elapsed.count() << " ms" << std::endl;
	}

private:
	std::chrono::high_resolution_clock::time_point mStartTime;
	std::string mTag;
};

#endif // __H_CAutoTicker__