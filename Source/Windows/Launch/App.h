#pragma once



class FApp
{
public:

	FApp();
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

private:

	/** Holds current delta time in seconds. */
	static double DeltaTime;
};

