#pragma once



class FApp
{
public:

	FApp() = default;
	/**
	 * Gets time delta in seconds.
	 *
	 * @return Time delta in seconds.
	 */
	static double GetDeltaTime()
	{
		return DeltaTime;
	}

	/**
	 * Sets time delta in seconds.
	 *
	 * @param seconds Time in seconds.
	 */
	static void SetDeltaTime(double Seconds)
	{
		DeltaTime = Seconds;
	}

	static void SetTotalTime(double Seconds)
	{
		TotalTime = Seconds;
	}

	static double GetTotalTime()
	{
		return TotalTime;
	}

	static void SetFramePerSeconds(double fps)
	{
		FramePerSeconds = fps;
	}

	static double GetFramePerSeconds()
	{
		return FramePerSeconds;
	}

	static void SetMiliSecondsPerFrame(double mspf)
	{
		MiliSecondsPerFrame = mspf;
	}

	static double GetMiliSecondsPerFrame()
	{
		return MiliSecondsPerFrame;
	}

private:

	static double FramePerSeconds;
	static double MiliSecondsPerFrame;
	static double TotalTime;
	/** Holds current delta time in seconds. */
	static double DeltaTime;
};

