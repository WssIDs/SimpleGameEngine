#include "Box.h"
#include "Graphics/DX11/Bindable/BindableBase.h"
#include "Graphics/DX11/Geometry/Cube.h"

Box::Box(Graphics& gfx,
	std::mt19937& rng,
	std::uniform_real_distribution<float>& adist,
	std::uniform_real_distribution<float>& ddist,
	std::uniform_real_distribution<float>& odist,
	std::uniform_real_distribution<float>& rdist,
	std::uniform_real_distribution<float>& bdist)
	:
	r(rdist(rng)),
	droll(ddist(rng)),
	dpitch(ddist(rng)),
	dyaw(ddist(rng)),
	dphi(odist(rng)),
	dtheta(odist(rng)),
	dchi(odist(rng)),
	phi(adist (rng)),
	theta(adist(rng)),
	chi(adist(rng))
{

	namespace dx = DirectX;

	if (!IsStaticInitialized())
	{
		struct Vertex
		{
			dx::XMFLOAT3 pos;
		};

		auto model = Cube::Make<Vertex>();
		model.Transform(dx::XMMatrixScaling(1.0f, 1.0f, 1.2f));

		AddStaticBind(std::make_unique<VertexBuffer>(gfx, model.m_vertices));

		auto pvs = std::make_unique<VertexShader>(gfx, L"..\\..\\..\\Shaders\\ColorIndexVS.cso");
		auto pvsbc = pvs->GetByteCode();
		AddStaticBind(std::move(pvs));

		AddStaticBind(std::make_unique<PixelShader>(gfx, L"..\\..\\..\\Shaders\\ColorIndexPS.cso"));

		AddStaticIndexBuffer(std::make_unique<IndexBuffer>(gfx, model.m_indices));

		struct PixelShaderConstants
		{
			struct
			{
				float r;
				float g;
				float b;
				float a;

			} face_colors[8];
		};


		const PixelShaderConstants cb2 =
		{
			{
				{1.0f,1.0f,1.0f},
				{1.0f,0.0f,0.0f},
				{0.0f,1.0f,0.0f},
				{0.0f,0.0f,1.0f},
				{1.0f,0.0f,1.0f},
				{0.0f,1.0f,1.0f},
				{0.0f,0.0f,0.0f},
			}
		};

		AddStaticBind(std::make_unique<PixelConstantBuffer<PixelShaderConstants>>(gfx, cb2));

		const std::vector<D3D11_INPUT_ELEMENT_DESC> ied =
		{
			{ "Position", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0	},
		};

		AddStaticBind(std::make_unique<InputLayout>(gfx, ied, pvsbc));
		AddStaticBind(std::make_unique<Topology>(gfx, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));
	}
	else
	{
		SetIndexFromStatic();
	}

	AddBind(std::make_unique<TransformConstantBuffer>(gfx, *this));

	// model deformation transform (per instance, not stored as bind)
	dx::XMStoreFloat3x3(
		&modelTransform,
		dx::XMMatrixScaling(1.0f, 1.0f, bdist(rng))
	);
}

void Box::Update(float deltaSeconds)
{
	roll += droll * deltaSeconds;
	pitch += dpitch * deltaSeconds;
	yaw += dyaw * deltaSeconds;
	theta += dtheta * deltaSeconds;
	phi += dphi * deltaSeconds;
	chi += dchi * deltaSeconds;
}

DirectX::XMMATRIX Box::GetTransformXM() const
{
	namespace dx = DirectX;

	return dx::XMLoadFloat3x3(&modelTransform) *
		dx::XMMatrixRotationRollPitchYaw(pitch, yaw, roll) *
		dx::XMMatrixTranslation(r, 0.0f, 0.0f) *
		dx::XMMatrixRotationRollPitchYaw(theta, phi, chi);
}
