#pragma once
#include <math.h>

constexpr float PI = 3.14159265f;
constexpr double PI_D = 3.1415926535897932;


struct WGMath
{

	// �������� � �������
	template<typename T>
	static FORCEINLINE auto Multiply(const T& x)
	{
		return x * x;
	}

	// �������� ���� ������
	template<typename T>
	static FORCEINLINE T WrapAngle(T theta)
	{
		const T modded = fmodf(theta, (T)2.0f * (T)PI);
		return (modded > (T)PI) ?
			(modded - (T)2.0f * (T)PI) :
			modded;
	}

	// ������������
	template<typename T>
	static FORCEINLINE T Interpolate(const T& src, const T& dst, float alpha)
	{
		return src + (dst - src) * alpha;
	}

	// ��������� � �������
	template<typename T>
	static FORCEINLINE T DegreesToRadians(T degValue)
	{
		return degValue * PI / (T)180.0;
	}
};