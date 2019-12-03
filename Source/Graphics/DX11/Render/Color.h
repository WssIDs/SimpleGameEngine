#pragma once



struct RGBAColor
{
	unsigned char R;
	unsigned char G;
	unsigned char B;
	unsigned char A;
};

struct LinearColor
{
	float R;
	float G;
	float B;
	float A;

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
};
