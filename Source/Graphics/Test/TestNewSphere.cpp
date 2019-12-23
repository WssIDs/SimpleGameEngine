#include "TestNewSphere.h"
#include "TestNewCube.h"

TestNewSphere::TestNewSphere()
{
	MeshMaterial = std::make_shared<Material>(BASE_SHADERS_DIR + "Sphere.fx");
	//MeshMaterial->AddTexture(BASE_TEXTURES_DIR + "main.dds");
	MeshMaterial->Load();

	std::vector<D3D11_INPUT_ELEMENT_DESC> ied =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
	};

	sphereData = Primitive::MakeSphere(12, 24);

	D3D11_BUFFER_DESC indexBufferDesc = {};
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned int) * (UINT)sphereData.Indices.size();
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA iinitData = {};
	iinitData.pSysMem = sphereData.Indices.data();
	Graphics::GetGraphics().pDevice3D->CreateBuffer(&indexBufferDesc, &iinitData, &IndexBuffer);

	D3D11_BUFFER_DESC vertextBufferDesc = {};
	vertextBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertextBufferDesc.ByteWidth = sizeof(SphereVertex) * (UINT)sphereData.Vertices.size();
	vertextBufferDesc.CPUAccessFlags = 0;
	vertextBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertextBufferDesc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA vertexBufferData = {};
	vertexBufferData.pSysMem = sphereData.Vertices.data();

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


	D3D11_RASTERIZER_DESC wfdesc;
	ZeroMemory(&wfdesc, sizeof(D3D11_RASTERIZER_DESC));
	wfdesc.FillMode = D3D11_FILL_SOLID;
	wfdesc.CullMode = D3D11_CULL_NONE;
	Graphics::GetGraphics().pDevice3D->CreateRasterizerState(&wfdesc, &RastrState);

	Graphics::GetGraphics().pDeviceContext3D->RSSetState(RastrState.Get());

	position = Position(0, 160, -30.0f);
}

void TestNewSphere::Update(float DeltaTime)
{

}

void TestNewSphere::Draw()
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

	if (!MeshMaterial->DiffuseSamplers.empty())
	{
		Graphics::GetGraphics().pDeviceContext3D->PSSetSamplers(0, (UINT)MeshMaterial->DiffuseSamplers.size(), MeshMaterial->DiffuseSamplers.data());
	}

	if (!MeshMaterial->DiffuseShaderResourceViews.empty())
	{
		Graphics::GetGraphics().pDeviceContext3D->PSSetShaderResources(0, (UINT)MeshMaterial->DiffuseShaderResourceViews.size(), MeshMaterial->DiffuseShaderResourceViews.data());
	}

	//Reset Vertex and Pixel Shaders
	Graphics::GetGraphics().pDeviceContext3D->IASetInputLayout(VertexLayout.Get());
	Graphics::GetGraphics().pDeviceContext3D->VSSetShader(MeshMaterial->VertexShader.Get(), nullptr, 0);
	Graphics::GetGraphics().pDeviceContext3D->PSSetShader(MeshMaterial->PixelShader.Get(), nullptr, 0);

	Graphics::GetGraphics().pDeviceContext3D->DrawIndexed((UINT)sphereData.Indices.size(), 0, 0);
}
