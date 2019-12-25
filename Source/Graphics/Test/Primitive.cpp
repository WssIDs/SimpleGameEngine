#include "Primitive.h"
#include "Imgui\imgui.h"

Primitive::Primitive()
{
	Scale = Vector::IdentityVector();
	Location = Vector::ZeroVector();
	Rotation = Rotator(0.0f, 0.f, 0.0f);
}

void Primitive::SetName(std::string Name)
{
	this->Name = Name;
}

void Primitive::InitMaterial()
{
	MeshMaterial = std::make_shared<Material>(BASE_SHADERS_DIR + ShaderName);
	//MeshMaterial->AddTexture(BASE_TEXTURES_DIR + "uv_pattern.dds");
	//MeshMaterial->AddTexture(BASE_TEXTURES_DIR + "main.dds");
	MeshMaterial->Load();
}

void Primitive::SetMaterial(std::string Name)
{
	ShaderName = Name;
}

void Primitive::Init()
{

	SetScale3D(Scale);
	SetRotation(Rotation);
	SetLocation(Location);

	D3D11_BUFFER_DESC indexBufferDesc = {};
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned int) * (UINT)MeshData.Indices.size();
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA iinitData = {};
	iinitData.pSysMem = MeshData.Indices.data();
	Graphics::GetGraphics().GetDevice3D()->CreateBuffer(&indexBufferDesc, &iinitData, &IndexBuffer);

	D3D11_BUFFER_DESC vertextBufferDesc = {};
	vertextBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertextBufferDesc.ByteWidth = sizeof(DirectX::XMFLOAT3) * (UINT)MeshData.Vertices.size();
	vertextBufferDesc.CPUAccessFlags = 0;
	vertextBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertextBufferDesc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA vertexBufferData = {};
	vertexBufferData.pSysMem = MeshData.Vertices.data();

	Graphics::GetGraphics().GetDevice3D()->CreateBuffer(&vertextBufferDesc, &vertexBufferData, &VertexBuffer);

	Graphics::GetGraphics().GetDevice3D()->CreateInputLayout(InputElement.data(), (UINT)InputElement.size(), MeshMaterial->VertexShaderBufferData.data(), MeshMaterial->VertexShaderBufferData.size(), &VertexLayout);

	D3D11_BUFFER_DESC cbbd;
	ZeroMemory(&cbbd, sizeof(D3D11_BUFFER_DESC));

	cbbd.Usage = D3D11_USAGE_DEFAULT;
	cbbd.ByteWidth = sizeof(ConstantTransform);
	cbbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbbd.CPUAccessFlags = 0;
	cbbd.MiscFlags = 0;

	Graphics::GetGraphics().GetDevice3D()->CreateBuffer(&cbbd, nullptr, &pConstantBufferPerObject);

	//ZeroMemory(&cbbd, sizeof(D3D11_BUFFER_DESC));

	//cbbd.Usage = D3D11_USAGE_DEFAULT;
	//cbbd.ByteWidth = sizeof(cbPerFrame);
	//cbbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	//cbbd.CPUAccessFlags = 0;
	//cbbd.MiscFlags = 0;

	//Graphics::GetGraphics().pDevice3D->CreateBuffer(&cbbd, nullptr, &pConstantBufferPerFrame);

	D3D11_RASTERIZER_DESC wfdesc;
	ZeroMemory(&wfdesc, sizeof(D3D11_RASTERIZER_DESC));
	wfdesc.FillMode = D3D11_FILL_SOLID;
	wfdesc.CullMode = D3D11_CULL_NONE;
	Graphics::GetGraphics().GetDevice3D()->CreateRasterizerState(&wfdesc, &RastrState);

	Graphics::GetGraphics().GetDeviceContext3D()->RSSetState(RastrState.Get());
}

void Primitive::Update(double DeltaTime)
{
	//SetScale3D(Scale);
	//SetLocation(Location);

	auto scale = DirectX::XMMatrixScaling(Scale.x, Scale.y, Scale.z);
	auto rotation = DirectX::XMMatrixRotationRollPitchYaw(Rotation.Pitch, Rotation.Yaw, Rotation.Roll);
	auto translate = DirectX::XMMatrixTranslation(Location.x, Location.y, Location.z);

	ModelView = scale * rotation * translate;
}

void Primitive::Draw()
{
	UINT stride = sizeof(DirectX::XMFLOAT3);
	UINT offset = 0;

	Graphics::GetGraphics().GetDeviceContext3D()->IASetVertexBuffers(0, 1, VertexBuffer.GetAddressOf(), &stride, &offset);
	Graphics::GetGraphics().GetDeviceContext3D()->IASetIndexBuffer(IndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
	Graphics::GetGraphics().GetDeviceContext3D()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	//const auto pos = DirectX::XMLoadFloat3(&light.Position);
	//DirectX::XMStoreFloat3(&light.Position, DirectX::XMVector3Transform(pos, Graphics::GetGraphics().GetCamera()));

	//constantBufferPerFrame.light = light;
	//Graphics::GetGraphics().pDeviceContext3D->UpdateSubresource(pConstantBufferPerFrame.Get(), 0, nullptr, &constantBufferPerFrame, 0, 0);
	//Graphics::GetGraphics().pDeviceContext3D->PSSetConstantBuffers(0, 1, pConstantBufferPerFrame.GetAddressOf());

	ConstantBufferTransformPerObj.ModelView = DirectX::XMMatrixTranspose(ModelView);
	ConstantBufferTransformPerObj.ModelViewProj = DirectX::XMMatrixTranspose(ModelView * Graphics::GetGraphics().GetCamera() * Graphics::GetGraphics().GetProjection());

	Graphics::GetGraphics().GetDeviceContext3D()->UpdateSubresource(pConstantBufferPerObject.Get(), 0, nullptr, &ConstantBufferTransformPerObj, 0, 0);
	Graphics::GetGraphics().GetDeviceContext3D()->VSSetConstantBuffers(0, 1, pConstantBufferPerObject.GetAddressOf());

	if (!MeshMaterial->DiffuseSamplers.empty())
	{
		Graphics::GetGraphics().GetDeviceContext3D()->PSSetSamplers(0, (UINT)MeshMaterial->DiffuseSamplers.size(), MeshMaterial->DiffuseSamplers.data());
	}

	if (!MeshMaterial->DiffuseShaderResourceViews.empty())
	{
		Graphics::GetGraphics().GetDeviceContext3D()->PSSetShaderResources(0, (UINT)MeshMaterial->DiffuseShaderResourceViews.size(), MeshMaterial->DiffuseShaderResourceViews.data());
	}

	//Reset Vertex and Pixel Shaders
	Graphics::GetGraphics().GetDeviceContext3D()->IASetInputLayout(VertexLayout.Get());
	Graphics::GetGraphics().GetDeviceContext3D()->VSSetShader(MeshMaterial->VertexShader.Get(), nullptr, 0);
	Graphics::GetGraphics().GetDeviceContext3D()->PSSetShader(MeshMaterial->PixelShader.Get(), nullptr, 0);

	Graphics::GetGraphics().GetDeviceContext3D()->DrawIndexed((UINT)MeshData.Indices.size(), 0, 0);

	//if (ImGui::Begin(Name.c_str()))
	//{
	//	ImGui::Text("Location");
	//	ImGui::SliderFloat("LocationX", &Location.x, -100.0f, 100.0f, "%.1f");
	//	ImGui::SliderFloat("LocationY", &Location.y, -100.0f, 200.0f, "%.1f");
	//	ImGui::SliderFloat("LocationZ", &Location.z, -100.0f, 100.0f, "%.1f");
	//	ImGui::Text("Scale3D");
	//	ImGui::SliderFloat("ScaleX", &Scale.x, 0.0f, 10.0f, "%.1f");
	//	ImGui::SliderFloat("ScaleY", &Scale.y, 0.0f, 10.0f, "%.1f");
	//	ImGui::SliderFloat("ScaleZ", &Scale.z, 0.0f, 10.0f, "%.1f");
	//}
	//ImGui::End();
}

void Primitive::SetScale3D(Vector inScale)
{
	Scale = inScale;
}

void Primitive::SetLocation(Vector newLocation)
{
	Location = newLocation;
}

void Primitive::SetRotation(Rotator newRotation)
{
	Rotation = newRotation;
}

Vector Primitive::GetLocation() const
{
	return Location;
}

Vector Primitive::GetScale3D() const
{
	return Scale;
}
