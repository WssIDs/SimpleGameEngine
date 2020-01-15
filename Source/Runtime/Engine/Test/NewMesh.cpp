#include "NewMesh.h"
#include "OpenFBX\ofbx.h"
#include <iterator>
#include <vector>
#include <algorithm>

DEFINE_LOG_CATEGORY(NewMeshLog)

NewMesh::NewMesh()
{
}

bool NewMesh::Init()
{

	LoadMesh();	
	InitMaterial();
	InitBufferData();

	return true;
}

bool NewMesh::LoadMesh()
{
	std::string Path(BASE_MODELS_DIR + MeshName.c_str());
	FILE* fp = nullptr;
	fopen_s(&fp, Path.c_str(), "rb");
	if (!fp) return false;

	fseek(fp, 0, SEEK_END);
	long file_size = ftell(fp);
	fseek(fp, 0, SEEK_SET);

	auto* content = new uint8[file_size];
	fread(content, 1, file_size, fp);
	ofbx::IScene* Scene = ofbx::load((uint8*)content, file_size, 0);

	if (!Scene)
	{
		WGE_LOG(NewMeshLog, LogVerbosity::Error, "Failed loading fbx scene");
	}

	InputElement.push_back({ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 });
	InputElement.push_back({ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 });
	InputElement.push_back({ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 28, D3D11_INPUT_PER_VERTEX_DATA, 0 });
	InputElement.push_back({ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 40, D3D11_INPUT_PER_VERTEX_DATA, 0 });

	std::vector<MainVertex> Vertx;

	WGE_LOG(NewMeshLog, LogVerbosity::Default, "Vertices");


	int vertex_count = Scene->getMesh(0)->getGeometry()->getVertexCount(); // 3;
	const ofbx::Vec3* vertices = Scene->getMesh(0)->getGeometry()->getVertices();
	const ofbx::Vec3* normals = Scene->getMesh(0)->getGeometry()->getNormals();
	const ofbx::Vec2* uvs = Scene->getMesh(0)->getGeometry()->getUVs();
	for (int i = 0; i < vertex_count; ++i)
	{
		MainVertex currentVertex;
		currentVertex.InitPosition(vertices[i].x, vertices[i].y, vertices[i].z);
		currentVertex.InitUV(uvs[i].y, uvs[i].x);
		currentVertex.InitColor(LinearColor::White);
		currentVertex.InitNormal(normals[i].x, normals[i].y, normals[i].z);
		Vertx.emplace_back(currentVertex);
	}

	const int* faceIndices = Scene->getMesh(0)->getGeometry()->getFaceIndices();
	int index_count = Scene->getMesh(0)->getGeometry()->getIndexCount();

	WGE_LOG(NewMeshLog, LogVerbosity::Default, "Indices");
	for (int i = 0; i < index_count; ++i)
	{
		int index = 0;
		if (faceIndices[i] < 0)
		{
			index = std::abs(faceIndices[i]) - 1;
		}
		else
		{
			index = faceIndices[i];
		}

		Indices.emplace_back(index);
	}

	MeshData = { Vertx, Indices };

	delete[] content;
	fclose(fp);

	return true;
}

void NewMesh::InitMaterial()
{
	MeshMaterial = std::make_shared<Material>(BASE_SHADERS_DIR + GetShaderName());
	//MeshMaterial->AddTexture(BASE_TEXTURES_DIR + "uv_pattern.dds");
	//MeshMaterial->AddTexture(BASE_TEXTURES_DIR + "main.dds");
	MeshMaterial->Load();
}

void NewMesh::Update(double DeltaTime)
{
	Primitive::Update(DeltaTime);
}

void NewMesh::Draw()
{
	Primitive::Draw();
}

void NewMesh::SetMesh(const std::string& FileName)
{
	MeshName = FileName;
}
