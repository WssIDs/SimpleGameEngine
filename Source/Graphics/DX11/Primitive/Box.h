#pragma once
#include "../../Tests/TestObject.h"
#include "../Bindable/ConstantBuffers.h"


class Box : public TestObject<Box>
{
public:
	Box(Graphics& gfx, std::mt19937& rng,
		std::uniform_real_distribution<float>& adist,
		std::uniform_real_distribution<float>& ddist,
		std::uniform_real_distribution<float>& odist,
		std::uniform_real_distribution<float>& rdist,
		std::uniform_real_distribution<float>& bdist,
		DirectX::XMFLOAT3 material);

	DirectX::XMMATRIX GetTransformXM() const override;
	/* Spawn imgui material control window
	 * return false if closed
	 */
	bool SpawnControlWindow(int id, Graphics& gfx);

private:
	void SyncMaterial(Graphics& gfx);
	
	struct PSMaterialConstant
	{
		DirectX::XMFLOAT3 color;
		float specularIntensity = 0.6f;
		float specularPower = 30.0f;
		float padding[3];
	} m_materialConstants;
	using MaterialContantBuffer = PixelConstantBuffer<PSMaterialConstant>;

private:
	// model transform
	DirectX::XMFLOAT3X3 modelTransform;
};

