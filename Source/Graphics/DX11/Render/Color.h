#pragma once
#include "boost\serialization\access.hpp"
#include <string>
#include <sstream>



struct RGBAColor
{
	unsigned char R = 0;
	unsigned char G = 0;
	unsigned char B = 0;
	unsigned char A = 0;

	RGBAColor()
	{
		R = 0;
		G = 0;
		B = 0;
		A = 0;
	}

	RGBAColor(unsigned char red, unsigned char green, unsigned char blue)
	{
		R = red;
		G = green;
		B = blue;
		A = 0;
	}

	RGBAColor(unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha)
		: RGBAColor(red, green, blue)
	{
		A = alpha;
	}

private:
	friend class boost::serialization::access;
	template <typename Archive>
	void serialize(Archive& ar, const unsigned int version)
	{
		ar& R;
		ar& G;
		ar& B;
		ar& A;
	}
};

struct LinearColor
{
	float R;
	float G;
	float B;
	float A;

	LinearColor()
	{
		R = 0.0f;
		G = 0.0f;
		B = 0.0f;
		A = 0.0f;
	}

	LinearColor(float red, float green, float blue, float alpha = 1.0f)
	{
		R = red;
		G = green;
		B = blue;
		A = alpha;
	}

	template <typename Archive>
	void serialize(Archive& ar, const unsigned int version)
	{
		ar& R & G & B & A;
	}

	std::string ToString() const
	{
		std::stringstream stream;
		stream << "(R=" << R << ",G=" << G << ",B=" << B << ",A=" << A;
		return stream.str();
	}

	static const LinearColor White;
	static const LinearColor Gray;
	static const LinearColor Black;
	static const LinearColor Transparent;
	static const LinearColor Red;
	static const LinearColor Green;
	static const LinearColor Blue;
	static const LinearColor Yellow;
};
