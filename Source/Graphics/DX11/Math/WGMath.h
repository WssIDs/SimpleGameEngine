#pragma once
#include <math.h>

constexpr float PI = 3.14159265f;
constexpr double PI_D = 3.1415926535897932;

// Возвести в квадрат
template<typename T>
constexpr auto sq(const T& x)
{
	return x * x;
}

// Получить угол охвата
template<typename T>
T wrap_angle(T theta)
{
	const T modded = fmodf(theta, (T)2.0f * (T)PI);
	return (modded > (T)PI) ?
		(modded - (T)2.0f * (T)PI) :
		modded;
}

// Интерполяция
template<typename T>
constexpr T interpolate(const T& src, const T& dst, float alpha)
{
	return src + (dst - src) * alpha;
}

// Перевести в радианы
template<typename T>
constexpr T to_rad(T deg)
{
	return deg * PI / (T)180.0;
}
