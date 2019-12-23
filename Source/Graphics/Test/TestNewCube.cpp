#include "TestNewCube.h"
#include "../Helpers/StringHelper.h"
#include "Material.h"
#include "Imgui/imgui.h"

TestNewCube::TestNewCube()
{
	MeshMaterial = std::make_shared<Material>(BASE_SHADERS_DIR + "Default.fx");
	MeshMaterial->AddTexture(BASE_TEXTURES_DIR + "uv_pattern.dds");
	//MeshMaterial->AddTexture(BASE_TEXTURES_DIR + "main.dds");
	MeshMaterial->Load();

	//MeshMaterial->Serialize();

	//if (MeshMaterial->IsLoaded())
	//{
	//	MeshMaterial->Serialize();
	//}

	//if (!MeshMaterial->IsLoaded())
	//{
	//	MeshMaterial = std::make_shared<Material>(BASE_SHADERS_DIR + "Default.fx");
	//	MeshMaterial->AddTexture(BASE_TEXTURES_DIR + "uv_pattern.dds");
	//	MeshMaterial->Load();

	//	//if (MeshMaterial->IsLoaded())
	//	//{
	//	//	MeshMaterial->Serialize();
	//	//}
	//}
	
	//MeshMaterial = std::make_shared<Material>();
	//MeshMaterial->Deserialize("Default.asset");

	//MeshMaterial->Load();

	std::vector<D3D11_INPUT_ELEMENT_DESC> ied =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 20, D3D11_INPUT_PER_VERTEX_DATA, 0},
	};

	// Front Face
	VertexData = { 
		// Front Face
		CubeVertex(-1.0f, -1.0f, -1.0f, 0.0f, 1.0f,-1.0f, -1.0f, -1.0f),
		CubeVertex(-1.0f,  1.0f, -1.0f, 0.0f, 0.0f,-1.0f,  1.0f, -1.0f),
		CubeVertex(1.0f,  1.0f, -1.0f, 1.0f, 0.0f, 1.0f,  1.0f, -1.0f),
		CubeVertex(1.0f, -1.0f, -1.0f, 1.0f, 1.0f, 1.0f, -1.0f, -1.0f),

		// Back Face
		CubeVertex(-1.0f, -1.0f, 1.0f, 1.0f, 1.0f,-1.0f, -1.0f, 1.0f),
		CubeVertex(1.0f, -1.0f, 1.0f, 0.0f, 1.0f, 1.0f, -1.0f, 1.0f),
		CubeVertex(1.0f,  1.0f, 1.0f, 0.0f, 0.0f, 1.0f,  1.0f, 1.0f),
		CubeVertex(-1.0f,  1.0f, 1.0f, 1.0f, 0.0f,-1.0f,  1.0f, 1.0f),

		// Top Face
		CubeVertex(-1.0f, 1.0f, -1.0f, 0.0f, 1.0f,-1.0f, 1.0f, -1.0f),
		CubeVertex(-1.0f, 1.0f,  1.0f, 0.0f, 0.0f,-1.0f, 1.0f,  1.0f),
		CubeVertex(1.0f, 1.0f,  1.0f, 1.0f, 0.0f, 1.0f, 1.0f,  1.0f),
		CubeVertex(1.0f, 1.0f, -1.0f, 1.0f, 1.0f, 1.0f, 1.0f, -1.0f),

		// Bottom Face
		CubeVertex(-1.0f, -1.0f, -1.0f, 1.0f, 1.0f,-1.0f, -1.0f, -1.0f),
		CubeVertex(1.0f, -1.0f, -1.0f, 0.0f, 1.0f, 1.0f, -1.0f, -1.0f),
		CubeVertex(1.0f, -1.0f,  1.0f, 0.0f, 0.0f, 1.0f, -1.0f,  1.0f),
		CubeVertex(-1.0f, -1.0f,  1.0f, 1.0f, 0.0f,-1.0f, -1.0f,  1.0f),

		// Left Face
		CubeVertex(-1.0f, -1.0f,  1.0f, 0.0f, 1.0f,-1.0f, -1.0f,  1.0f),
		CubeVertex(-1.0f,  1.0f,  1.0f, 0.0f, 0.0f,-1.0f,  1.0f,  1.0f),
		CubeVertex(-1.0f,  1.0f, -1.0f, 1.0f, 0.0f,-1.0f,  1.0f, -1.0f),
		CubeVertex(-1.0f, -1.0f, -1.0f, 1.0f, 1.0f,-1.0f, -1.0f, -1.0f),

		// Right Face
		CubeVertex(1.0f, -1.0f, -1.0f, 0.0f, 1.0f, 1.0f, -1.0f, -1.0f),
		CubeVertex(1.0f,  1.0f, -1.0f, 0.0f, 0.0f, 1.0f,  1.0f, -1.0f),
		CubeVertex(1.0f,  1.0f,  1.0f, 1.0f, 0.0f, 1.0f,  1.0f,  1.0f),
		CubeVertex(1.0f, -1.0f,  1.0f, 1.0f, 1.0f, 1.0f, -1.0f,  1.0f),
	};

	// Front Face
	IndicesData =
	{
		// Front Face
		0,  1,  2,
		0,  2,  3,

		// Back Face
		4,  5,  6,
		4,  6,  7,

		// Top Face
		8,  9, 10,
		8, 10, 11,

		// Bottom Face
		12, 13, 14,
		12, 14, 15,

		// Left Face
		16, 17, 18,
		16, 18, 19,

		// Right Face
		20, 21, 22,
		20, 22, 23
	};


	D3D11_BUFFER_DESC indexBufferDesc = {};
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned int) * (UINT)IndicesData.size();
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA iinitData = {};
	iinitData.pSysMem = IndicesData.data();
	Graphics::GetGraphics().pDevice3D->CreateBuffer(&indexBufferDesc, &iinitData, &IndexBuffer);

	D3D11_BUFFER_DESC vertextBufferDesc = {};
	vertextBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertextBufferDesc.ByteWidth = sizeof(CubeVertex) * (UINT)VertexData.size();
	vertextBufferDesc.CPUAccessFlags = 0;
	vertextBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertextBufferDesc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA vertexBufferData = {};
	vertexBufferData.pSysMem = VertexData.data();

	Graphics::GetGraphics().pDevice3D->CreateBuffer(&vertextBufferDesc, &vertexBufferData, &VertexBuffer);

	Graphics::GetGraphics().pDevice3D->CreateInputLayout(ied.data(), (UINT)ied.size(), MeshMaterial->VertexShaderBufferData.data(), MeshMaterial->VertexShaderBufferData.size(), &VertexLayout);

	D3D11_BUFFER_DESC cbbd;
	ZeroMemory(&cbbd, sizeof(D3D11_BUFFER_DESC));

	cbbd.Usage = D3D11_USAGE_DEFAULT;
	cbbd.ByteWidth = sizeof(Transforms);
	cbbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbbd.CPUAccessFlags = 0;
	cbbd.MiscFlags = 0;

	Graphics::GetGraphics().pDevice3D->CreateBuffer(&cbbd, nullptr, &pConstantBufferPerObject);

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
	Graphics::GetGraphics().pDevice3D->CreateRasterizerState(&wfdesc, &RastrState);

	Graphics::GetGraphics().pDeviceContext3D->RSSetState(RastrState.Get());

	position = Position(0, 160, -30.0f);
}

void TestNewCube::Update(float DeltaTime)
{
	//DirectX::XMVECTOR lightVector = DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);

	//lightVector = DirectX::XMVector3TransformCoord(lightVector, cube1World);

	//light.Position.x = DirectX::XMVectorGetX(lightVector);
	//light.Position.y = DirectX::XMVectorGetY(lightVector);
	//light.Position.z = DirectX::XMVectorGetZ(lightVector);
}

void TestNewCube::Draw()
{
	UINT stride = sizeof(CubeVertex);
	UINT offset = 0;

	Graphics::GetGraphics().pDeviceContext3D->IASetVertexBuffers(0, 1, VertexBuffer.GetAddressOf(), &stride, &offset);
	Graphics::GetGraphics().pDeviceContext3D->IASetIndexBuffer(IndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
	Graphics::GetGraphics().pDeviceContext3D->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	//const auto pos = DirectX::XMLoadFloat3(&light.Position);
	//DirectX::XMStoreFloat3(&light.Position, DirectX::XMVector3Transform(pos, Graphics::GetGraphics().GetCamera()));

	//constantBufferPerFrame.light = light;
	//Graphics::GetGraphics().pDeviceContext3D->UpdateSubresource(pConstantBufferPerFrame.Get(), 0, nullptr, &constantBufferPerFrame, 0, 0);
	//Graphics::GetGraphics().pDeviceContext3D->PSSetConstantBuffers(0, 1, pConstantBufferPerFrame.GetAddressOf());

	constantBufferPerObj.ModelView = DirectX::XMMatrixTranspose(DirectX::XMMatrixTranslation(position.x, position.y, position.z));
	constantBufferPerObj.ModelViewProj = DirectX::XMMatrixTranspose(DirectX::XMMatrixTranslation(position.x, position.y, position.z) * Graphics::GetGraphics().GetCamera() * Graphics::GetGraphics().GetProjection());

	Graphics::GetGraphics().pDeviceContext3D->UpdateSubresource(pConstantBufferPerObject.Get(), 0, nullptr, &constantBufferPerObj, 0, 0);
	Graphics::GetGraphics().pDeviceContext3D->VSSetConstantBuffers(0, 1, pConstantBufferPerObject.GetAddressOf());

	Graphics::GetGraphics().pDeviceContext3D->PSSetSamplers(0, (UINT)MeshMaterial->DiffuseSamplers.size(), MeshMaterial->DiffuseSamplers.data());
	Graphics::GetGraphics().pDeviceContext3D->PSSetShaderResources(0, (UINT)MeshMaterial->DiffuseShaderResourceViews.size(), MeshMaterial->DiffuseShaderResourceViews.data());

	//Reset Vertexand Pixel Shaders
	Graphics::GetGraphics().pDeviceContext3D->IASetInputLayout(VertexLayout.Get());
	Graphics::GetGraphics().pDeviceContext3D->VSSetShader(MeshMaterial->VertexShader.Get(), nullptr, 0);
	Graphics::GetGraphics().pDeviceContext3D->PSSetShader(MeshMaterial->PixelShader.Get(), nullptr, 0);

	Graphics::GetGraphics().pDeviceContext3D->DrawIndexed((UINT)IndicesData.size(), 0, 0);

	if (ImGui::Begin("TestCube"))
	{
		ImGui::Text("Position");
		ImGui::SliderFloat("X", &position.x, -100.0f, 100.0f, "%.1f");
		ImGui::SliderFloat("Y", &position.y, -100.0f, 200.0f, "%.1f");
		ImGui::SliderFloat("Z", &position.z, -100.0f, 100.0f, "%.1f");
	}
	ImGui::End();
}



//Graphics::GetGraphics().rot += deltaTime;
////if(Graphics::GetGraphics().rot > 6.28f)
////{
////	Graphics::GetGraphics().rot = 0.0f;
////}
//
//Graphics::GetGraphics().cube1World = DirectX::XMMatrixIdentity();
//
////Define cube1's world space matrix
//DirectX::XMVECTOR rotaxis = DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
//Graphics::GetGraphics().Rotation = DirectX::XMMatrixRotationAxis(rotaxis, -Graphics::GetGraphics().rot);
//Graphics::GetGraphics().Scale = DirectX::XMMatrixScaling(1.3f, 1.3f, 1.3f);
//
////Set cube1's world space using the transformations
//Graphics::GetGraphics().cube1World = Graphics::GetGraphics().Rotation * Graphics::GetGraphics().Scale;