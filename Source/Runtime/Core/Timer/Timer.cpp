#include "Timer.h"
#include <Windows.h>
#include <stdexcept>
#include "..\..\Launch\App.h"

using namespace std::chrono;


FTimer::FTimer()
	:
	startTime(0),
	totalIdleTime(0),
	pausedTime(0),
	currentTime(0),
	previousTime(0),
	secondsPerCount(0.0),
	deltaTime(0.0),
	bStopped(false),
	TotalTime(0.0),
	DefaultDeltaTime(1.0 / (double)240),
	MaxSkipFrames(10),
	FramePerSeconds(0),
	MiliSecondsPerFrame(0.0)
{
	// get the frequency of the PerformanceCounter
	long long int frequency = 0;
	if (QueryPerformanceFrequency((LARGE_INTEGER*)&frequency))
	{
		// compute the secondsPerCount as the reciprocal of the frequency
		secondsPerCount = 1.0 / (double)frequency;
	}
	else
		// the hardware does not support a high-precision timer -> throw an error
		throw std::runtime_error("The hardware does not support a high-precision timer!");

	last = steady_clock::now();
}

void FTimer::CalculateFramePerSeconds()
{
	nFrames++;

	// compute average statistics over one second
	if ((GetTotalTime() - ElapsedTime) >= 1.0)
	{
		// set fps and mspf
		FramePerSeconds = nFrames;
		MiliSecondsPerFrame = 1000.0 / (double)FramePerSeconds;

		FApp::SetFramePerSeconds(FramePerSeconds);
		FApp::SetMiliSecondsPerFrame(MiliSecondsPerFrame);

		// reset
		nFrames = 0;
		ElapsedTime += 1.0;
	}
}

double FTimer::GetDefaultDeltaTime() const
{
	return DefaultDeltaTime;
}

int FTimer::GetMaxSkipFrames() const
{
	return MaxSkipFrames;
}

void FTimer::AccumulateTime(bool bIncrease)
{
	if (bIncrease)
	{
		AccumulatedTime += GetDeltaTime();
	}
	else
	{
		AccumulatedTime -= GetDefaultDeltaTime();
	}
}

void FTimer::GenerateNumberLoops()
{
	NumberLoops++;
}

void FTimer::ResetAccumulatedTime()
{
	AccumulatedTime = 0;
}

void FTimer::ResetNumberLoops()
{
	NumberLoops = 0;
}

double FTimer::GetAccumulatedTime() const
{
	return AccumulatedTime;
}

int FTimer::GetNumberLoops() const
{
	return NumberLoops;
}

FTimer* FTimer::Get()
{
	static FTimer Instance;
	return &Instance;
}

double FTimer::GetTotalTime() const
{
	return TotalTime;
}

double FTimer::GetDeltaTime() const
{
	return deltaTime;
}

void FTimer::Start()
{
	// this function starts the timer (if it is not already running)
	if (bStopped)
	{
		long long int now = 0;
		if (QueryPerformanceCounter((LARGE_INTEGER*)&now))
		{
			// add the duration of the pause to the total idle time
			totalIdleTime += (now - pausedTime);

			// set the previous time to the current time
			previousTime = now;

			// reset the pausedTime to 0 and isStopped to false
			pausedTime = 0;
			bStopped = false;
		}
		else
			// unable to query the performance counter, throw an error
			throw std::runtime_error("Unable to query the performance counter!");
	}
}

void FTimer::Reset()
{
	// this function resets the timer
	long long int now = 0;
	if (QueryPerformanceCounter((LARGE_INTEGER*)&now))
	{
		startTime = now;
		previousTime = now;
		pausedTime = 0;
		bStopped = false;

		// return success
		return;
	}
	else
		// unable to query the performance counter, throw an error
		throw std::runtime_error("Unable to query the performance counter!");
}

void FTimer::Tick()
{
	// this function lets the timer tick, i.e. it computes the time that has elapsed between two frames
	if (bStopped)
	{
		// if the game is stopped, the elapsed time is obviously 0
		deltaTime = 0.0;
	}
	else
	{
		// get the current time
		if (QueryPerformanceCounter((LARGE_INTEGER*)&currentTime))
		{
			// compute the time elapsed since the previous frame
			deltaTime = (currentTime - previousTime) * secondsPerCount;

			// set previousTime to crrentTime, as in the next tick, this frame will be the previous frame
			previousTime = currentTime;

			// deltaTime can be negative if the processor goes idle for example
			if (deltaTime < 0.0)
				deltaTime = 0.0;
		}
		else
			// unable to query the performance counter, throw an error
			throw std::runtime_error("Unable to query the performance counter!");
	}

	FApp::SetDeltaTime(deltaTime);

	// this function returns the total time since the game started: (t_now - t_start) - t_totalIdle
	if (bStopped)
	{
		TotalTime = (pausedTime - startTime - totalIdleTime) * secondsPerCount;
	}
	else
	{
		TotalTime = (currentTime - startTime - totalIdleTime) * secondsPerCount;
	}

	FApp::SetTotalTime(TotalTime);
}

void FTimer::Stop()
{
	// this function stops the timer (if it is currently running)
	if (!bStopped)
	{
		long long int now = 0;
		if (QueryPerformanceCounter((LARGE_INTEGER*)&now))
		{
			// set the time the timer was stopped to "now"
			pausedTime = now;
			bStopped = true;
		}
		else
			// unable to query the performance counter, throw an error
			throw std::runtime_error("Unable to query the performance counter!");
	}
}
