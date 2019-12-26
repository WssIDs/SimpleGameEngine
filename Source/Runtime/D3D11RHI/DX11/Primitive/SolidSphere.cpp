#include "SolidSphere.h"
#include <Runtime/D3D11RHI/DX11/Bindable/BindableCommon.h>
#include <Runtime/D3D11RHI/DX11/Geometry/Sphere.h>

SolidSphere::SolidSphere(Graphics& gfx, float radius)
{
	using namespace Bind;
	namespace dx = DirectX;

	auto model = Sphere::Make();
	model.Transform(dx::XMMatrixScaling(radius, radius, radius));

	const auto geometryTag = "$sphere." + std::to_string(radius);
	AddBind(VertexBuffer::Resolve(gfx,geometryTag, model.vertices));
	AddBind(IndexBuffer::Resolve(gfx, geometryTag, model.indices));

	auto pvs = VertexShader::Resolve(gfx, R"(..\..\..\Shaders\SolidVS.cso)");
	auto pvsbc = pvs->GetByteCode();
	AddBind(std::move(pvs));

	AddBind(PixelShader::Resolve(gfx, R"(..\..\..\Shaders\SolidPS.cso)"));

	struct PSColorConstant
	{
		dx::XMFLOAT3 color = { 1.0f,1.0f,1.0f };
		float padding;
	} colorConst;

	AddBind(PixelConstantBuffer<PSColorConstant>::Resolve(gfx, colorConst, 1u));


	AddBind(InputLayout::Resolve(gfx, model.vertices.GetLayout(), pvsbc));

	AddBind(Topology::Resolve(gfx, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));

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