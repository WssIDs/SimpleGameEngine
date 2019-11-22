#pragma once
#include "TestObject.h"



class AssetTest : public TestObject<AssetTest>
{
public:
	AssetTest(Graphics& gfx, std::mt19937& rng,
		std::uniform_real_distribution<float>& adist,
		std::uniform_real_distribution<float>& ddist,
		std::uniform_real_distribution<float>& odist,
		std::uniform_real_distribution<float>& rdist,
		DirectX::XMFLOAT3 material,
		DirectX::XMFLOAT3 scale);
};

