#pragma once
#include <math.h>
#include <Windows.h>

constexpr float PI = 3.14159265f;
constexpr double PI_D = 3.1415926535897932;


struct WGMath
{

	// Возвести в квадрат
	template<typename T>
	static FORCEINLINE T Square(const T& X)
	{
		return X * X;
	}

	// Возвести в степень
	template<typename T>
	static FORCEINLINE auto Pow(const T& X, int Exp)
	{
		return pow(X,Exp);
	}

	// Получить угол охвата
	template<typename T>
	static FORCEINLINE T WrapAngle(T theta)
	{
		constexpr T twoPi = (T)2 * (T)PI_D;
		const T mod = fmod(theta, twoPi);
		if (mod > (T)PI_D)
		{
			return mod - twoPi;
		}
		else if (mod < (T)PI_D)
		{
			return mod + twoPi;
		}
		return mod;
	}

	// Интерполяция
	template<typename T>
	static FORCEINLINE T Interpolate(const T& src, const T& dst, float alpha)
	{
		return src + (dst - src) * alpha;
	}

	/** Interpolate float from Current to Target with constant step */
	static float FInterpConstantTo(float Current, float Target, float DeltaTime, float InterpSpeed)
	{
		const float Dist = Target - Current;

		// If distance is too small, just set the desired location
		if (WGMath::Square(Dist) < FLT_MIN)
		{
			return Target;
		}

		const float Step = InterpSpeed * DeltaTime;
		return Current + WGMath::Clamp<float>(Dist, -Step, Step);
	}

	/** Interpolate float from Current to Target. Scaled by distance to Target, so it has a strong start speed and ease out. */
	static float FInterpTo(float Current, float Target, float DeltaTime, float InterpSpeed)
	{
		// If no interp speed, jump to target value
		if (InterpSpeed <= 0.f)
		{
			return Target;
		}

		// Distance to reach
		const float Dist = Target - Current;

		// If distance is too small, just set the desired location
		if (WGMath::Square(Dist) < FLT_MIN)
		{
			return Target;
		}

		// Delta Move, Clamp so we do not over shoot.
		const float DeltaMove = Dist * WGMath::Clamp<float>(DeltaTime * InterpSpeed, 0.f, 1.f);

		return Current + DeltaMove;
	}

	template<typename T>
	static FORCEINLINE T Clamp(const T& X, T Min, T Max)
	{
		return X < Min ? Min : X < Max ? X : Max;
	}

	template<typename T>
	static FORCEINLINE T MapRangeClamped(const T& value, const T& inMin, const T& inMax, const T& outMin, const T& outMax)
	{
		return (value - inMin) * (outMax - outMin) / (inMax - inMin) + outMin;
	}

	// Перевести в радианы
	template<typename T>
	static FORCEINLINE T DegreesToRadians(T degValue)
	{
		return degValue * PI / (T)180.0;
	}
};