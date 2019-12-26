#include "Primitive.h"

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
	Graphics::Get().GetDevice3D()->CreateBuffer(&indexBufferDesc, &iinitData, &pIndexBuffer);

	D3D11_BUFFER_DESC vertextBufferDesc = {};
	vertextBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertextBufferDesc.ByteWidth = sizeof(MainVertex) * (UINT)MeshData.Vertices.size();
	vertextBufferDesc.CPUAccessFlags = 0;
	vertextBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertextBufferDesc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA vertexBufferData = {};
	vertexBufferData.pSysMem = MeshData.Vertices.data();

	Graphics::Get().GetDevice3D()->CreateBuffer(&vertextBufferDesc, &vertexBufferData, &pVertexBuffer);
	Graphics::Get().GetDevice3D()->CreateInputLayout(InputElement.data(), (UINT)InputElement.size(), MeshMaterial->VertexShaderBufferData.data(), MeshMaterial->VertexShaderBufferData.size(), &VertexLayout);

	TransformConstantBuffer = std::make_shared<VertexBuffer>(ConstantBufferTransformPerObj);
	ColorConstantBuffer = std::make_shared<PixelBuffer>(ConstantBufferColor,1);

	D3D11_RASTERIZER_DESC wfdesc;
	ZeroMemory(&wfdesc, sizeof(D3D11_RASTERIZER_DESC));
	wfdesc.FillMode = D3D11_FILL_SOLID;
	wfdesc.CullMode = D3D11_CULL_NONE;
	Graphics::Get().GetDevice3D()->CreateRasterizerState(&wfdesc, &RastrState);

	Graphics::Get().GetDeviceContext3D()->RSSetState(RastrState.Get());
}

void Primitive::Update(double DeltaTime)
{
	auto scale = DirectX::XMMatrixScaling(Scale.x, Scale.y, Scale.z);
	auto rotation = DirectX::XMMatrixRotationRollPitchYaw(Rotation.Pitch, Rotation.Yaw, Rotation.Roll);
	auto translate = DirectX::XMMatrixTranslation(Location.x, Location.y, Location.z);

	ModelView = scale * rotation * translate;

	ConstantBufferTransformPerObj.ModelView = DirectX::XMMatrixTranspose(ModelView);
	ConstantBufferTransformPerObj.ModelViewProj = DirectX::XMMatrixTranspose(ModelView * Graphics::Get().GetCamera() * Graphics::Get().GetProjection());
}

void Primitive::Draw()
{
	UINT stride = sizeof(MainVertex);
	UINT offset = 0;

	Graphics::Get().GetDeviceContext3D()->IASetVertexBuffers(0, 1, pVertexBuffer.GetAddressOf(), &stride, &offset);
	Graphics::Get().GetDeviceContext3D()->IASetIndexBuffer(pIndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
	Graphics::Get().GetDeviceContext3D()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	TransformConstantBuffer->Update(ConstantBufferTransformPerObj);
	ColorConstantBuffer->Update(ConstantBufferColor);

	if (!MeshMaterial->DiffuseSamplers.empty())
	{
		Graphics::Get().GetDeviceContext3D()->PSSetSamplers(0, (UINT)MeshMaterial->DiffuseSamplers.size(), MeshMaterial->DiffuseSamplers.data());
	}

	if (!MeshMaterial->DiffuseShaderResourceViews.empty())
	{
		Graphics::Get().GetDeviceContext3D()->PSSetShaderResources(0, (UINT)MeshMaterial->DiffuseShaderResourceViews.size(), MeshMaterial->DiffuseShaderResourceViews.data());
	}

	//Reset Vertex and Pixel Shaders
	Graphics::Get().GetDeviceContext3D()->IASetInputLayout(VertexLayout.Get());
	Graphics::Get().GetDeviceContext3D()->VSSetShader(MeshMaterial->VertexShader.Get(), nullptr, 0);
	Graphics::Get().GetDeviceContext3D()->PSSetShader(MeshMaterial->PixelShader.Get(), nullptr, 0);

	Graphics::Get().GetDeviceContext3D()->DrawIndexed((UINT)MeshData.Indices.size(), 0, 0);
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

Rotator Primitive::GetRotation() const
{
	return Rotation;
}

Vector Primitive::GetScale3D() const
{
	return Scale;
}
