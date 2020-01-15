#pragma once

using uint8 = unsigned char;		// 8-bit  unsigned.
using uint16 = unsigned short;		// 16-bit unsigned.
using uint32 = unsigned int;		// 32-bit unsigned.
using uint64 = unsigned long long;	// 64-bit unsigned.

using int8 = signed char;			// 8-bit  signed.
using int16 = signed short int;		// 16-bit signed.
using int32 = signed int;			// 32-bit signed.
using int64 = signed long long;		// 64-bit signed.


// Character types.
using ANSICHAR = char;		// An ANSI character       -                  8-bit fixed-width representation of 7-bit characters.
using WIDECHAR = wchar_t;	// A wide character        - In-memory only.  ?-bit fixed-width representation of the platform's natural wide character set.  Could be different sizes on different platforms.
using CHAR8 = uint8;		// An 8-bit character type - In-memory only.  8-bit representation.  Should really be char8_t but making this the generic option is easier for compilers which don't fully support C++11 yet (i.e. MSVC).
using CHAR16 = uint16;		// A 16-bit character type - In-memory only.  16-bit representation.  Should really be char16_t but making this the generic option is easier for compilers which don't fully support C++11 yet (i.e. MSVC).
using CHAR32 = uint32;		// A 32-bit character type - In-memory only.  32-bit representation.  Should really be char32_t but making this the generic option is easier for compilers which don't fully support C++11 yet (i.e. MSVC).
//using TCHAR = WIDECHAR;		// A switchable character  - In-memory only.  Either ANSICHAR or WIDECHAR, depending on a licensee's requirements.



struct Vector
{
	Vector()
		:
		x(0.0f),
		y(0.0f),
		z(0.0f)
	{}

	Vector(float inX, float inY, float inZ)
		:
		x(inX),
		y(inY),
		z(inZ)
	{}

	Vector(double inX, double inY, double inZ)
		:
		x((float)inX),
		y((float)inY),
		z((float)inZ)
	{}

	static Vector IdentityVector()
	{
		return Vector(1.0f, 1.0f, 1.0f);
	}

	static Vector ZeroVector()
	{
		return Vector(0.0f, 0.0f, 0.0f);
	}

	//const char* ToString()
	//{
	//	ss << "{x = " << x << ", y = " << y << ", z = " << z << "}";

	//	return ss.str().c_str();
	//}

	friend Vector operator+(const Vector& leftVector, const Vector& rightVector)
	{
		Vector result = Vector(leftVector.x + rightVector.x, leftVector.y + rightVector.y, leftVector.z + rightVector.z);
		return result;
	}

	bool operator==(const Vector& Other)
	{
		return (x == Other.x && y == Other.y && z == Other.z) ? true : false;
	}

	float x;
	float y;
	float z;
};

struct Vector2D
{
	Vector2D()
		:
		x(0.0f),
		y(0.0f)
	{}

	Vector2D(float inX, float inY)
		:
		x(inX),
		y(inY)
	{}

	Vector2D(double inX, double inY)
		:
		x((float)inX),
		y((float)inY)
	{}

	static Vector2D IdentityVector()
	{
		return Vector2D(1.0f, 1.0f);
	}

	static Vector2D ZeroVector()
	{
		return Vector2D(0.0f, 0.0f);
	}

	friend Vector2D operator+(const Vector2D& leftVector, const Vector2D& rightVector)
	{
		Vector2D result = Vector2D(leftVector.x + rightVector.x, leftVector.y + rightVector.y);
		return result;
	}

	float x;
	float y;
};

struct Rotator
{
	Rotator()
		:
		Pitch(0.0f),
		Yaw(0.0f),
		Roll(0.0f)
	{}

	Rotator(float inPitch, float inYaw, float inRoll)
		:
		Pitch(inPitch),
		Roll(inRoll),
		Yaw(inYaw)
	{}

	static Rotator IdentityRotator()
	{
		return Rotator(1.0f, 1.0f, 1.0f);
	}

	static Rotator ZeroRotator()
	{
		Vector(0.0f, 0.0f, 0.0f);
	}

	float Pitch;
	float Roll;
	float Yaw;
};