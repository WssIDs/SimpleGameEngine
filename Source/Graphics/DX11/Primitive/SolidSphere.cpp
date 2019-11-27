#include "SolidSphere.h"
#include "..\Bindable\BindableCommon.h"
#include "..\Geometry\Sphere.h"

SolidSphere::SolidSphere(Graphics& gfx, float radius)
{
	using namespace Bind;
	namespace dx = DirectX;

	auto model = Sphere::Make();
	model.Transform(dx::XMMatrixScaling(radius, radius, radius));

	AddBind(std::make_shared<VertexBuffer>(gfx, model.vertices));
	AddBind(std::make_shared<class IndexBuffer>(gfx, model.indices));

	auto pvs = std::make_shared<VertexShader>(gfx, R"(..\..\..\Shaders\SolidVS.cso)");
	auto pvsbc = pvs->GetByteCode();
	AddBind(std::move(pvs));

	AddBind(std::make_shared<PixelShader>(gfx, TEXT("..\\..\\..\\Shaders\\SolidPS.cso")));

	struct PSColorConstant
	{
		dx::XMFLOAT3 color = { 1.0f,1.0f,1.0f };
		float padding;
	} colorConst;

	AddBind(std::make_shared < PixelConstantBuffer<PSColorConstant>>(gfx, colorConst));


	AddBind(std::make_shared<InputLayout>(gfx, model.vertices.GetLayout().GetD3DLayout(), pvsbc));

	AddBind(std::make_shared<Topology>(gfx, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));

	AddBind(std::make_shared<TransformConstantBuffer>(gfx, *this));
}

void SolidSphere::Update(float deltaSeconds) {}

void SolidSphere::SetPosition(DirectX::XMFLOAT3 position)
{
	pos = position;
}

DirectX::XMMATRIX SolidSphere::GetTransformXM() const
{
	return DirectX::XMMatrixTranslation(pos.x, pos.y, pos.z);
}