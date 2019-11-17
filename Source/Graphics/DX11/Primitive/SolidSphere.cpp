#include "SolidSphere.h"
#include "..\Bindable\BindableBase.h"
#include "..\Geometry\Sphere.h"

SolidSphere::SolidSphere(Graphics& gfx, float radius)
{
	namespace dx = DirectX;

	if (!IsStaticInitialized())
	{
		struct Vertex
		{
			dx::XMFLOAT3 pos;
		};

		auto model = Sphere::Make<Vertex>();
		model.Transform(dx::XMMatrixScaling(radius,radius,radius));


		AddBind(std::make_unique<VertexBuffer>(gfx, model.m_vertices));
		AddIndexBuffer(std::make_unique<class IndexBuffer>(gfx, model.m_indices));

		auto pvs = std::make_unique<VertexShader>(gfx, L"..\\..\\..\\Shaders\\SolidVS.cso");
		auto pvsbc = pvs->GetByteCode();
		AddStaticBind(std::move(pvs));

		AddStaticBind(std::make_unique<PixelShader>(gfx, L"..\\..\\..\\Shaders\\SolidPS.cso"));

		struct PSColorConstant
		{
			dx::XMFLOAT3 color = { 1.0f,1.0f,1.0f };
			float padding;
		} colorConst;

		AddStaticBind(std::make_unique < PixelConstantBuffer<PSColorConstant>>(gfx, colorConst));

		const std::vector<D3D11_INPUT_ELEMENT_DESC> ied =
		{
			{ "Position",0,DXGI_FORMAT_R32G32B32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA,0 },
		};
		AddStaticBind(std::make_unique<InputLayout>(gfx, ied, pvsbc));

		AddStaticBind(std::make_unique<Topology>(gfx, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));
	}
	else
	{
		SetIndexFromStatic();
	}

	AddBind(std::make_unique<TransformConstantBuffer>(gfx, *this));
}

void SolidSphere::Update(float deltaSeconds) {}

void SolidSphere::SetPosition(DirectX::XMFLOAT3 position)
{
	m_pos = position;
}

DirectX::XMMATRIX SolidSphere::GetTransformXM() const
{
	return DirectX::XMMatrixTranslation(m_pos.x, m_pos.y, m_pos.z);
}