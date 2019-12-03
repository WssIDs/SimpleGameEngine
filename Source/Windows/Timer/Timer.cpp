#include "Timer.h"
#include <Windows.h>
#include <stdexcept>

using namespace std::chrono;


Timer::Timer()
	:
	startTime(0),
	totalIdleTime(0),
	pausedTime(0),
	currentTime(0),
	previousTime(0),
	secondsPerCount(0.0),
	deltaTime(0.0),
	bStopped(false)
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

float Timer::Mark()
{
	const auto old = last;
	last = steady_clock::now();
	const duration<float> frametime = last - old;
	return frametime.count();
}

float Timer::Peek() const
{
	return duration<float>(steady_clock::now() - last).count();
}

double Timer::getTotalTime() const
{
	// this function returns the total time since the game started: (t_now - t_start) - t_totalIdle
	if (bStopped)
		return (pausedTime - startTime - totalIdleTime) * secondsPerCount;
	else
		return (currentTime - startTime - totalIdleTime) * secondsPerCount;
}

double Timer::getDeltaTime() const
{
	return deltaTime;
}

void Timer::start()
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

void Timer::reset()
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

void Timer::tick()
{
	// this function lets the timer tick, i.e. it computes the time that has elapsed between two frames
	if (bStopped)
	{
		// if the game is stopped, the elapsed time is obviously 0
		deltaTime = 0.0;

		// return success
		return;
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
}

void Timer::stop()
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
