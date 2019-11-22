#include "Box.h"
#include "Graphics/DX11/Bindable/BindableBase.h"
#include "Graphics/DX11/Geometry/Cube.h"
#include "Imgui/imgui.h"

Box::Box(Graphics& gfx,
	std::mt19937& rng,
	std::uniform_real_distribution<float>& adist,
	std::uniform_real_distribution<float>& ddist,
	std::uniform_real_distribution<float>& odist,
	std::uniform_real_distribution<float>& rdist,
	std::uniform_real_distribution<float>& bdist,
	DirectX::XMFLOAT3 material)
	:
	TestObject(gfx,rng,adist,ddist,odist,rdist)
{

	namespace dx = DirectX;

	if (!IsStaticInitialized())
	{
		struct Vertex
		{
			dx::XMFLOAT3 pos;
			dx::XMFLOAT3 n;
		};

		auto model = Cube::MakeIndependent<Vertex>();
		model.SetNormalsIndependentFlat();

		AddStaticBind(std::make_unique<VertexBuffer>(gfx, model.vertices));

		auto pvs = std::make_unique<VertexShader>(gfx, TEXT("..\\..\\..\\Shaders\\PhongVS.cso"));
		auto pvsbc = pvs->GetByteCode();
		AddStaticBind(std::move(pvs));

		AddStaticBind(std::make_unique<PixelShader>(gfx, TEXT("..\\..\\..\\Shaders\\PhongPS.cso")));

		AddStaticIndexBuffer(std::make_unique<IndexBuffer>(gfx, model.indices));

		const std::vector<D3D11_INPUT_ELEMENT_DESC> ied =
		{
			{ "Position", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{ "Normal", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0},
		};

		AddStaticBind(std::make_unique<InputLayout>(gfx, ied, pvsbc));
		AddStaticBind(std::make_unique<Topology>(gfx, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));
	}
	else
	{
		SetIndexFromStatic();
	}

	AddBind(std::make_unique<TransformConstantBuffer>(gfx, *this));

	materialConstants.color = material;
	AddBind(std::make_unique<MaterialContantBuffer>(gfx, materialConstants, 1u));

	// model deformation transform (per instance, not stored as bind)
	dx::XMStoreFloat3x3(
		&modelTransform,
		dx::XMMatrixScaling(1.0f, 1.0f, bdist(rng))
	);
}

DirectX::XMMATRIX Box::GetTransformXM() const
{
	namespace dx = DirectX;
	return dx::XMLoadFloat3x3(&modelTransform) * TestObject::GetTransformXM();
}

bool Box::SpawnControlWindow(int id, Graphics& gfx)
{
	using namespace std::string_literals;

	bool bDirty = false;
	bool bOpen = true;
	if (ImGui::Begin(("Box "s + std::to_string(id)).c_str(),&bOpen))
	{
		ImGui::Text("Material Properties");
		const auto cd = ImGui::ColorEdit3("Material Color", &materialConstants.color.x);
		const auto sid = ImGui::SliderFloat("Specular Intensity", &materialConstants.specularIntensity, 0.05f, 4.0f, "%.2f", 2);
		const auto spd = ImGui::SliderFloat("Specular Power", &materialConstants.specularPower, 1.0f, 200.0f, "%.2f", 2);
		bDirty = cd || sid || spd;

		ImGui::Text("Position");
		ImGui::SliderFloat("R", &r, 0.0f, 80.0f, "%.1f");
		ImGui::SliderAngle("Theta", &theta, -180.0f, 180.0f);
		ImGui::SliderAngle("Phi", &phi, -180.0f, 180.0f);
		ImGui::Text("Orientation");
		ImGui::SliderAngle("Roll", &roll, -180.0f, 180.0f);
		ImGui::SliderAngle("Pitch", &pitch, -180.0f, 180.0f);
		ImGui::SliderAngle("Yaw", &yaw, -180.0f, 180.0f);
	}
	ImGui::End();

	if (bDirty)
	{
		SyncMaterial(gfx);
	}

	return bOpen;
}

void Box::SyncMaterial(Graphics& gfx)
{
	auto pConstPS = QueryBindable<MaterialContantBuffer>();
	assert(pConstPS != nullptr);
	pConstPS->Update(gfx, materialConstants);
}
