#pragma once
#include <chrono>

class Timer
{
public:
	Timer();
	float Mark();
	float Peek() const;

	// getters: return time measured in seconds
	double getTotalTime() const;		// returns the total time the game has been running (minus paused time)
	double getDeltaTime() const;		// returns the time between two frames

	// methods
	void start();		// starts the timer, called each time the game is unpaused
	void reset();		// sets the counter to zero, called once before message loop
	void tick();		// called every frame, lets the time tick
	void stop();		// called when the game is paused

private:
	// times measured in counts
	long long int startTime;			// time at the start of the application
	long long int totalIdleTime;		// total time the game was idle
	long long int pausedTime;			// time at the moment the game was paused last
	long long int currentTime;			// stores the current time; i.e. time at the current frame
	long long int previousTime;		    // stores the time at the last inquiry before current; i.e. time at the previous frame

	// times measured in seconds
	double secondsPerCount;			    // reciprocal of the frequency, computed once at the initialization of the class
	double deltaTime;					// time between two frames, updated during the game loop

	// state of the timer
	bool bStopped;					    // true iff the timer is stopped

	std::chrono::steady_clock::time_point last;
};

