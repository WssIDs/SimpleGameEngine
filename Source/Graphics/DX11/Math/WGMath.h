#pragma once
#include <math.h>

constexpr float PI = 3.14159265f;
constexpr double PI_D = 3.1415926535897932;


struct WGMath
{

	// Возвести в квадрат
	template<typename T>
	static FORCEINLINE auto Multiply(const T& x)
	{
		return x * x;
	}

	// Возвести в степень
	template<typename T>
	static FORCEINLINE auto Pow(const T& x, int exp)
	{
		return pow(x,exp);
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