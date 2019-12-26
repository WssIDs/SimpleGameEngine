#pragma once
#include <chrono>
#include <memory>

class FTimer
{
public:
	FTimer(const FTimer&) = delete;
	FTimer& operator=(const FTimer&) = delete;
	~FTimer()
	{
	};

	static FTimer* Get();

	// getters: return time measured in seconds
	double GetTotalTime() const;		// returns the total time the game has been running (minus paused time)
	double GetDeltaTime() const;		// returns the time between two frames

	// methods
	void Start();		// starts the timer, called each time the game is unpaused
	void Reset();		// sets the counter to zero, called once before message loop
	void Tick();		// called every frame, lets the time tick
	void Stop();		// called when the game is paused

	void CalculateFramePerSeconds();

	double GetDefaultDeltaTime() const;
	int GetMaxSkipFrames() const;

	void AccumulateTime(bool bIncrease);
	void GenerateNumberLoops();
	void ResetAccumulatedTime();
	void ResetNumberLoops();

	double GetAccumulatedTime() const;
	int GetNumberLoops() const;

protected:
	FTimer();
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
	double TotalTime;

	// state of the timer
	bool bStopped;					    // true if the timer is stopped

	//std::shared_ptr<FTimer> timer;
	int FramePerSeconds = 0;							// frames per second
	double MiliSecondsPerFrame = 0.0;					    // milliseconds per frame

	int nFrames = 0;				    // number of frames seen
	double ElapsedTime = 0.0;		    // time since last call

	double AccumulatedTime = 0.0;		// stores the time accumulated by the rendered
	int NumberLoops = 0;				// the number of completed loops while updating the game

	double DefaultDeltaTime;
	int MaxSkipFrames;

	std::chrono::steady_clock::time_point last;
};

