#pragma once
#include "boost\serialization\access.hpp"



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

	LinearColor(float red, float green, float blue)
	{
		R = red;
		G = green;
		B = blue;
		A = 1.0f;
	}

	LinearColor(float red, float green, float blue, float alpha)
		: LinearColor(red,green,blue)
	{
		A = alpha;
	}

	template <typename Archive>
	void serialize(Archive& ar, const unsigned int version)
	{
		ar& R;
		ar& G;
		ar& B;
		ar& A;
	}
};
