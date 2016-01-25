#include "Clock.h"

#include <Utils/Assert.h>

void Clock::Reset() {
	mFrequency = Frequency();
	mTotalTime = 0.0;
	mElapsedTime = 0.0;
	GetTime(mStartTime);
	mCurrentTime = mStartTime;
	mLastTime = mCurrentTime;
	mFrameCount = 0;
	mFrameRate = 0;
	mLastTotalElapsedTime = 0.0;
}

double Clock::Frequency() const {
	LARGE_INTEGER frequency;
	const BOOL success = QueryPerformanceFrequency(&frequency);
	ASSERT(success);
	return static_cast<double>(frequency.QuadPart);
}

void Clock::UpdateTime() {
	GetTime(mCurrentTime);
	mTotalTime = (mCurrentTime.QuadPart - mStartTime.QuadPart) / mFrequency;
	mElapsedTime = (mCurrentTime.QuadPart - mLastTime.QuadPart) / mFrequency;
	mLastTime = mCurrentTime;
	mLastTotalElapsedTime += mElapsedTime;
	if (mLastTotalElapsedTime >= 1.0) {
		mLastTotalElapsedTime = 0.0;
		mFrameRate = mFrameCount;
		mFrameCount = 0;
	}
	++mFrameCount;
}