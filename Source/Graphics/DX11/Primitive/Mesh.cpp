#include "Mesh.h"
#include <unordered_map>

#include "..\Bindable\Texture.h"
#include "..\Bindable\Sampler.h"
#include "..\Render\Vertex.h"

#include <assimp\Importer.hpp>
#include <assimp\scene.h>
#include <assimp\postprocess.h>
#include <assimp\material.h>

DEFINE_LOG_CATEGORY(MeshLog)

namespace dx = DirectX;

// Mesh
Mesh::Mesh(Graphics& gfx, std::vector<std::shared_ptr<Bind::Bindable>> bindPtrs)
{
	AddBind(Bind::Topology::Resolve(gfx, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));

	for (auto& pb : bindPtrs)
	{
		AddBind(std::move(pb));
	}

	AddBind(std::make_shared<Bind::TransformConstantBuffer>(gfx, *this));
}

void Mesh::Draw(Graphics& gfx, DirectX::FXMMATRIX accumulatedTransform) const
{
	DirectX::XMStoreFloat4x4(&transform, accumulatedTransform);
	Drawable::Draw(gfx);
}

DirectX::XMMATRIX Mesh::GetTransformXM() const
{
	return DirectX::XMLoadFloat4x4(&transform);
}


// Node
Node::Node(int id, const std::string& name, std::vector<Mesh*> meshPtrs, const DirectX::XMMATRIX& inTransform)
	:
	id(id),
	meshPtrs(std::move(meshPtrs)),
	name(name)
{
	DirectX::XMStoreFloat4x4(&transform, inTransform);
	DirectX::XMStoreFloat4x4(&appliedTransform, dx::XMMatrixIdentity());
}

void Node::Draw(Graphics& gfx, DirectX::FXMMATRIX accumulatedTransform) const
{
	const auto built = 
		DirectX::XMLoadFloat4x4(&appliedTransform) *
		DirectX::XMLoadFloat4x4(&transform) *
		accumulatedTransform;

	for (const auto pm : meshPtrs)
	{
		pm->Draw(gfx, built);
	}
	for (const auto& pc : childPtrs)
	{
		pc->Draw(gfx, built);
	}
}

void Node::SetTransform(DirectX::FXMMATRIX inTransform)
{
	dx::XMStoreFloat4x4(&transform, inTransform);
}

int Node::GetId() const
{
	return id;
}

void Node::ShowTree(Node*& pSelectedNode) const
{
	const int selectedId = (pSelectedNode == nullptr) ? -1 : pSelectedNode->GetId();

	const auto nodeFlags = ImGuiTreeNodeFlags_OpenOnArrow
		| ((GetId() == selectedId) ? ImGuiTreeNodeFlags_Selected : 0)
		| ((childPtrs.empty()) ? ImGuiTreeNodeFlags_Leaf : 0);

	// if tree node expanded, recursively render all children
	const auto expanded = ImGui::TreeNodeEx((void*)(intptr_t)GetId(), nodeFlags, name.c_str());
	if (ImGui::IsItemClicked())
	{
		pSelectedNode = const_cast<Node*>(this);
	}
	if (expanded)
	{
		for (const auto& pChild : childPtrs)
		{
			pChild->ShowTree(pSelectedNode);
		}
		ImGui::TreePop();
	}
}

void Node::AddChild(std::unique_ptr<Node> pChild)
{
	assert(pChild);
	childPtrs.push_back(std::move(pChild));
}


// Model window
class ModelWindow // pImpl idiom, only defined in this .cpp
{
public:
	void Show(Graphics& gfx, const char* windowName, const Node& root) noexcept
	{
		// window name defaults to "Model"
		windowName = windowName ? windowName : "Model";

		int nodeIndex = 0;
		if (ImGui::Begin(windowName))
		{
			ImGui::Columns(2, nullptr, true);
			root.ShowTree(pSelectedNode);

			ImGui::NextColumn();

			if (pSelectedNode != nullptr)
			{
				auto& transform = transforms[pSelectedNode->GetId()];

				ImGui::Text("Orientation");
				ImGui::SliderAngle("Roll", &transform.roll, -180.0f, 180.0f);
				ImGui::SliderAngle("Pitch", &transform.pitch, -180.0f, 180.0f);
				ImGui::SliderAngle("Yaw", &transform.yaw, -180.0f, 180.0f);
				ImGui::Text("Position");
				ImGui::SliderFloat("X", &transform.x, -20.0f, 20.0f);
				ImGui::SliderFloat("Y", &transform.y, -20.0f, 20.0f);
				ImGui::SliderFloat("Z", &transform.z, -20.0f, 20.0f);

				if( !pSelectedNode->ControlMaterial(gfx, fullTexturedMaterial))
				{
					pSelectedNode->ControlMaterial(gfx, noTexturedMaterial);
				}
			}
		}
		ImGui::End();
	}
	dx::XMMATRIX GetTransform() const noexcept
	{
		const auto& transform = transforms.at(pSelectedNode->GetId());
		return dx::XMMatrixRotationRollPitchYaw(transform.roll, transform.pitch, transform.yaw) *
			dx::XMMatrixTranslation(transform.x, transform.y, transform.z);
	}

	Node*  GetSelectedNode() const
	{
		return pSelectedNode;
	}

private:
	Node* pSelectedNode = nullptr;
	struct TransformParameters
	{
		float roll = 0.0f;
		float pitch = 0.0f;
		float yaw = 0.0f;
		float x = 0.0f;
		float y = 0.0f;
		float z = 0.0f;
	};

	Node::PSMaterialConstantFull fullTexturedMaterial;
	Node::PSMaterialConstantNotex noTexturedMaterial;
	std::unordered_map<int, TransformParameters> transforms;
};


// Model
Model::Model(Graphics& gfx, const std::string path, dx::XMFLOAT3 scale3D)
	:
	pWindow(std::make_unique<ModelWindow>())
{
	Assimp::Importer imp;
	const auto pScene = imp.ReadFile(path.c_str(),
		aiProcess_Triangulate |
		aiProcess_JoinIdenticalVertices |
		aiProcess_ConvertToLeftHanded |
		aiProcess_GenNormals |
		aiProcess_CalcTangentSpace
	);

	if(pScene)
	{
		WGE_LOG(MeshLog, LogVerbosity::Success, "Loaded Scene from file = %s", path.c_str());

		WGE_LOG(MeshLog, LogVerbosity::Default, "Loading Meshes", path.c_str());

		for (size_t i = 0; i < pScene->mNumMeshes; i++)
		{
			meshPtrs.push_back(ParseMesh(gfx, *pScene->mMeshes[i], pScene->mMaterials, scale3D));
		}

		int nextId = 0;
		pRoot = ParseNode(nextId, *pScene->mRootNode);
		WGE_LOG(MeshLog, LogVerbosity::Success, "Loaded Meshes", path.c_str());
		WGE_LOG(MeshLog, LogVerbosity::Default, "----------------------------------");
	}
	else
	{
		WGE_LOG(MeshLog, LogVerbosity::Error, "Failed Loading Scene from file = %s", path.c_str());
	}
}

std::unique_ptr<Mesh> Model::ParseMesh(Graphics& gfx, const aiMesh& mesh, const aiMaterial* const* pMaterials, dx::XMFLOAT3 scale3D)
{
	WGE_LOG(MeshLog, LogVerbosity::Default, "----------------------------------");
	WGE_LOG(MeshLog, LogVerbosity::Default, "Start parse mesh = %s", mesh.mName.C_Str());

	using DynamicVtx::VertexLayout;

	std::vector<std::shared_ptr<Bind::Bindable>> bindablePtrs;

	bool bDiffuseMap = false;
	bool bDiffuseMapAlpha = false;

	bool bSpecularMap = false;
	bool bSpecularMapAlpha = false;
	
	bool bNormalMap = false;
	bool bNormalMapAlpha = false;

	bool bGlossinesMap = false;
	bool bGlossinesMapAlpha = false;
	
	float shininess = 2.0f;
	dx::XMFLOAT4 specularColor = { 0.18f,0.18f,0.18f,1.0f };
	dx::XMFLOAT4 diffuseColor = { 0.45f,0.45f,0.85f,1.0f };

	if (mesh.mMaterialIndex >= 0)
	{
		auto& material = *pMaterials[mesh.mMaterialIndex];
		aiString texFileName;
		
		if (material.GetTexture(aiTextureType_DIFFUSE, 0, &texFileName) == aiReturn_SUCCESS)
		{
			WGE_LOG(MeshLog, LogVerbosity::Success, "DiffuseMap loaded");

			auto DiffuseTexture = Bind::Texture::Resolve(gfx, BASE_MODELS_DIR + texFileName.C_Str());
			if(DiffuseTexture->HasAlpha())
			{
				bDiffuseMapAlpha = true;

				WGE_LOG(MeshLog, LogVerbosity::Success, "DiffuseMapAlpha loaded");
			}
			else
			{
				WGE_LOG(MeshLog, LogVerbosity::Warning, "DiffuseMapAlpha not loaded");
			}

			bindablePtrs.push_back(std::move(DiffuseTexture));
			bDiffuseMap = true;
		}
		else
		{
			WGE_LOG(MeshLog, LogVerbosity::Warning, "DiffuseMap not loaded");
			WGE_LOG(MeshLog, LogVerbosity::Default, "Try to loading diffuse color");

			if(material.Get(AI_MATKEY_COLOR_DIFFUSE, reinterpret_cast<aiColor3D&>(diffuseColor)) == aiReturn_SUCCESS)
			{
				WGE_LOG(MeshLog, LogVerbosity::Success, "Diffuse color loaded");
			}
			else
			{
				WGE_LOG(MeshLog, LogVerbosity::Warning, "Diffuse color not loaded, will be used default color");
			}
		}

		if (material.GetTexture(aiTextureType_SPECULAR, 0, &texFileName) == aiReturn_SUCCESS)
		{
			WGE_LOG(MeshLog, LogVerbosity::Success, "SpecularMap loaded");

			auto SpecularTexture = Bind::Texture::Resolve(gfx, BASE_MODELS_DIR + texFileName.C_Str(), 1);
			if (SpecularTexture->HasAlpha())
			{
				bSpecularMapAlpha = true;
				WGE_LOG(MeshLog, LogVerbosity::Success, "SpecularMapAlpha loaded");
			}
			else
			{
				WGE_LOG(MeshLog, LogVerbosity::Warning, "SpecularMapAlpha not loaded");
			}

			bindablePtrs.push_back(std::move(SpecularTexture));
			bSpecularMap = true;
		}
		else
		{
			WGE_LOG(MeshLog, LogVerbosity::Warning, "SpecularMap not loaded");
			WGE_LOG(MeshLog, LogVerbosity::Default, "Try to loading specular color");

			if (material.Get(AI_MATKEY_COLOR_SPECULAR, reinterpret_cast<aiColor3D&>(specularColor)) == aiReturn_SUCCESS)
			{
				WGE_LOG(MeshLog, LogVerbosity::Success, "Specular color loaded");
			}
			else
			{
				WGE_LOG(MeshLog, LogVerbosity::Warning, "Specular color not loaded, will be used default color");
			}
		}

		if (material.GetTexture(aiTextureType_SHININESS, 0, &texFileName) == aiReturn_SUCCESS)
		{
			WGE_LOG(MeshLog, LogVerbosity::Success, "GlossinessMap loaded");

			auto GlossinessTexture = Bind::Texture::Resolve(gfx, BASE_MODELS_DIR + texFileName.C_Str(), 2);
			if (GlossinessTexture->HasAlpha())
			{
				bGlossinesMapAlpha = true;
				WGE_LOG(MeshLog, LogVerbosity::Success, "GlossinessMapAlpha loaded");
			}
			else
			{
				WGE_LOG(MeshLog, LogVerbosity::Warning, "GlossinessMapAlpha not loaded");
			}

			material.Get(AI_MATKEY_SHININESS, shininess);

			bindablePtrs.push_back(std::move(GlossinessTexture));
			bGlossinesMap = true;
		}
		else
		{
			WGE_LOG(MeshLog, LogVerbosity::Warning, "GlossinessMap not loaded");
		}

		if (material.GetTexture(aiTextureType_NORMALS, 0, &texFileName) == aiReturn_SUCCESS)
		{
			WGE_LOG(MeshLog, LogVerbosity::Success, "NormalMap loaded");

			auto NormalTexture = Bind::Texture::Resolve(gfx, BASE_MODELS_DIR + texFileName.C_Str(), 3);
			if (NormalTexture->HasAlpha())
			{
				bNormalMapAlpha = true;
				WGE_LOG(MeshLog, LogVerbosity::Success, "NormalMapAlpha loaded");
			}
			else
			{
				WGE_LOG(MeshLog, LogVerbosity::Warning, "NormalMapAlpha not loaded");
			}

			bindablePtrs.push_back(std::move(NormalTexture));
			bNormalMap = true;
		}
		else
		{
			WGE_LOG(MeshLog, LogVerbosity::Warning, "NormalMap not loaded");
		}

		if (bDiffuseMap || bNormalMap || bSpecularMap)
		{
			bindablePtrs.push_back(Bind::Sampler::Resolve(gfx));
		}
	}
	else
	{
		WGE_LOG(MeshLog, LogVerbosity::Warning, "Mesh - % hasn't materials", mesh.mName.C_Str());
	}

	auto meshTag = BASE_MODELS_DIR + "%" + mesh.mName.C_Str();

	if(bDiffuseMap && bNormalMap && bSpecularMap)
	{
		WGE_LOG(MeshLog, LogVerbosity::Default, "Select init mesh mode = DiffuseNormalSpecular");

		DynamicVtx::VertexBuffer vertexBuffer(std::move(
			VertexLayout{}
			.Append(DynamicVtx::ElementType::Position3D)
			.Append(DynamicVtx::ElementType::Normal)
			.Append(DynamicVtx::ElementType::Tangent)
			.Append(DynamicVtx::ElementType::Bitangent)
			.Append(DynamicVtx::ElementType::Texture2D)
		));

		for (unsigned int i = 0; i < mesh.mNumVertices; i++)
		{
			vertexBuffer.EmplaceBack(
				dx::XMFLOAT3(mesh.mVertices[i].x * scale3D.x, mesh.mVertices[i].y * scale3D.y, mesh.mVertices[i].z * scale3D.z),
				*reinterpret_cast<dx::XMFLOAT3*>(&mesh.mNormals[i]),
				*reinterpret_cast<dx::XMFLOAT3*>(&mesh.mTangents[i]),
				*reinterpret_cast<dx::XMFLOAT3*>(&mesh.mBitangents[i]),
				*reinterpret_cast<dx::XMFLOAT2*>(&mesh.mTextureCoords[0][i])
			);
		}

		std::vector<unsigned short> indices;
		int indicesReserved = mesh.mNumFaces * 3;
		indices.reserve(indicesReserved);
		for (unsigned int i = 0; i < mesh.mNumFaces; i++)
		{
			const auto& face = mesh.mFaces[i];
			assert(face.mNumIndices == 3);
			indices.push_back(face.mIndices[0]);
			indices.push_back(face.mIndices[1]);
			indices.push_back(face.mIndices[2]);
		}

		bindablePtrs.push_back(Bind::VertexBuffer::Resolve(gfx, meshTag, vertexBuffer));

		bindablePtrs.push_back(Bind::IndexBuffer::Resolve(gfx, meshTag, indices));

		auto pvs = Bind::VertexShader::Resolve(gfx, BASE_SHADERS_DIR + "PhongVSNormalMap.cso");
		auto pvsbc = pvs->GetByteCode();
		bindablePtrs.push_back(std::move(pvs));

		bindablePtrs.push_back(Bind::PixelShader::Resolve(gfx, BASE_SHADERS_DIR + "PhongPSSpecularNormalMap.cso"));

		bindablePtrs.push_back(Bind::InputLayout::Resolve(gfx, vertexBuffer.GetLayout(), pvsbc));

		Node::PSMaterialConstantFull pmc;

		pmc.specularPower = shininess;
		//pmc.specularMapAlpha = bSpecularMapAlpha ? TRUE : FALSE;
		pmc.glossinesMapEnabled = bGlossinesMap ? TRUE : FALSE;

		bindablePtrs.push_back(Bind::PixelConstantBuffer<Node::PSMaterialConstantFull>::Resolve(gfx, pmc, 1u));
	}
	else if (bDiffuseMap && bNormalMap)
	{
		WGE_LOG(MeshLog, LogVerbosity::Default, "Select init mesh mode = DiffuseNormal");

		DynamicVtx::VertexBuffer vertexBuffer(std::move(
			VertexLayout{}
			.Append(DynamicVtx::ElementType::Position3D)
			.Append(DynamicVtx::ElementType::Normal)
			.Append(DynamicVtx::ElementType::Tangent)
			.Append(DynamicVtx::ElementType::Bitangent)
			.Append(DynamicVtx::ElementType::Texture2D)
		));

		for (unsigned int i = 0; i < mesh.mNumVertices; i++)
		{
			vertexBuffer.EmplaceBack(
				dx::XMFLOAT3(mesh.mVertices[i].x* scale3D.x, mesh.mVertices[i].y* scale3D.y, mesh.mVertices[i].z* scale3D.z),
				*reinterpret_cast<dx::XMFLOAT3*>(&mesh.mNormals[i]),
				*reinterpret_cast<dx::XMFLOAT3*>(&mesh.mTangents[i]),
				*reinterpret_cast<dx::XMFLOAT3*>(&mesh.mBitangents[i]),
				*reinterpret_cast<dx::XMFLOAT2*>(&mesh.mTextureCoords[0][i])
			);
		}

		std::vector<unsigned short> indices;
		int indicesReserved = mesh.mNumFaces * 3;
		indices.reserve(indicesReserved);
		for (unsigned int i = 0; i < mesh.mNumFaces; i++)
		{
			const auto& face = mesh.mFaces[i];
			assert(face.mNumIndices == 3);
			indices.push_back(face.mIndices[0]);
			indices.push_back(face.mIndices[1]);
			indices.push_back(face.mIndices[2]);
		}

		bindablePtrs.push_back(Bind::VertexBuffer::Resolve(gfx, meshTag, vertexBuffer));

		bindablePtrs.push_back(Bind::IndexBuffer::Resolve(gfx, meshTag, indices));

		auto pvs = Bind::VertexShader::Resolve(gfx, BASE_SHADERS_DIR + "PhongVSNormalMap.cso");
		auto pvsbc = pvs->GetByteCode();
		bindablePtrs.push_back(std::move(pvs));

		bindablePtrs.push_back(Bind::PixelShader::Resolve(gfx, BASE_SHADERS_DIR + "PhongPSNormalMap.cso"));

		bindablePtrs.push_back(Bind::InputLayout::Resolve(gfx, vertexBuffer.GetLayout(), pvsbc));

		struct PSMaterialConstantDiffuseNormal
		{
			float specularIntensity;
			float specularPower;
			BOOL  normalMapEnabled = TRUE;
			float padding[1];
		} pmc;

		pmc.specularPower = shininess;
		pmc.specularIntensity = (specularColor.x + specularColor.y + specularColor.z) / 3.0f;
		bindablePtrs.push_back(Bind::PixelConstantBuffer<PSMaterialConstantDiffuseNormal>::Resolve(gfx, pmc, 1u));
	}
	else if (bDiffuseMap)
	{
		WGE_LOG(MeshLog, LogVerbosity::Default, "Select init mesh mode = Diffuse");

		DynamicVtx::VertexBuffer vertexBuffer(std::move(
			VertexLayout{}
			.Append(DynamicVtx::ElementType::Position3D)
			.Append(DynamicVtx::ElementType::Normal)
			.Append(DynamicVtx::ElementType::Texture2D)
		));

		for (unsigned int i = 0; i < mesh.mNumVertices; i++)
		{
			vertexBuffer.EmplaceBack(
				dx::XMFLOAT3(mesh.mVertices[i].x* scale3D.x, mesh.mVertices[i].y* scale3D.y, mesh.mVertices[i].z* scale3D.z),
				*reinterpret_cast<dx::XMFLOAT3*>(&mesh.mNormals[i]),
				*reinterpret_cast<dx::XMFLOAT2*>(&mesh.mTextureCoords[0][i])
			);
		}

		std::vector<unsigned short> indices;
		int indicesReserved = mesh.mNumFaces * 3;
		indices.reserve(indicesReserved);
		for (unsigned int i = 0; i < mesh.mNumFaces; i++)
		{
			const auto& face = mesh.mFaces[i];
			assert(face.mNumIndices == 3);
			indices.push_back(face.mIndices[0]);
			indices.push_back(face.mIndices[1]);
			indices.push_back(face.mIndices[2]);
		}

		bindablePtrs.push_back(Bind::VertexBuffer::Resolve(gfx, meshTag, vertexBuffer));

		bindablePtrs.push_back(Bind::IndexBuffer::Resolve(gfx, meshTag, indices));

		auto pvs = Bind::VertexShader::Resolve(gfx, BASE_SHADERS_DIR + "PhongVS.cso");
		auto pvsbc = pvs->GetByteCode();
		bindablePtrs.push_back(std::move(pvs));

		bindablePtrs.push_back(Bind::PixelShader::Resolve(gfx, BASE_SHADERS_DIR + "PhongPS.cso"));

		bindablePtrs.push_back(Bind::InputLayout::Resolve(gfx, vertexBuffer.GetLayout(), pvsbc));

		struct PSMaterialConstantDiffuse
		{
			float specularIntensity;
			float specularPower;
			float padding[2];
		} pmc;

		pmc.specularPower = shininess;
		pmc.specularIntensity = (specularColor.x + specularColor.y + specularColor.z) / 3.0f;
		bindablePtrs.push_back(Bind::PixelConstantBuffer<PSMaterialConstantDiffuse>::Resolve(gfx, pmc, 1u));
	}
	else if (!bDiffuseMap && !bNormalMap && !bSpecularMap)
	{
		WGE_LOG(MeshLog, LogVerbosity::Default, "Select init mesh mode = NoTextures");

		DynamicVtx::VertexBuffer vertexBuffer(std::move(
			VertexLayout{}
			.Append(DynamicVtx::ElementType::Position3D)
			.Append(DynamicVtx::ElementType::Normal)
		));

		for (unsigned int i = 0; i < mesh.mNumVertices; i++)
		{
			vertexBuffer.EmplaceBack(
				dx::XMFLOAT3(mesh.mVertices[i].x* scale3D.x, mesh.mVertices[i].y* scale3D.y, mesh.mVertices[i].z* scale3D.z),
				*reinterpret_cast<dx::XMFLOAT3*>(&mesh.mNormals[i])
			);
		}

		std::vector<unsigned short> indices;
		int indicesReserved = mesh.mNumFaces * 3;
		indices.reserve(indicesReserved);
		for (unsigned int i = 0; i < mesh.mNumFaces; i++)
		{
			const auto& face = mesh.mFaces[i];
			assert(face.mNumIndices == 3);
			indices.push_back(face.mIndices[0]);
			indices.push_back(face.mIndices[1]);
			indices.push_back(face.mIndices[2]);
		}

		bindablePtrs.push_back(Bind::VertexBuffer::Resolve(gfx, meshTag, vertexBuffer));

		bindablePtrs.push_back(Bind::IndexBuffer::Resolve(gfx, meshTag, indices));

		auto pvs = Bind::VertexShader::Resolve(gfx, BASE_SHADERS_DIR + "PhongVSNotex.cso");
		auto pvsbc = pvs->GetByteCode();
		bindablePtrs.push_back(std::move(pvs));

		bindablePtrs.push_back(Bind::PixelShader::Resolve(gfx, BASE_SHADERS_DIR + "PhongPSNotex.cso"));

		bindablePtrs.push_back(Bind::InputLayout::Resolve(gfx, vertexBuffer.GetLayout(), pvsbc));

		Node::PSMaterialConstantNotex pmc;

		pmc.specularPower = shininess;
		pmc.specularColor = specularColor;
		pmc.materialColor = diffuseColor;
		bindablePtrs.push_back(Bind::PixelConstantBuffer<Node::PSMaterialConstantNotex>::Resolve(gfx, pmc, 1u));
	}
	else
	{
		WGE_LOG(MeshLog, LogVerbosity::Fatal, "Terrible combination of textures in material smh");
		throw std::runtime_error("Terrible combination of textures in material smh");
	}

	WGE_LOG(MeshLog, LogVerbosity::Default, "End parse mesh = %s", mesh.mName.C_Str());

	return std::make_unique<Mesh>(gfx, std::move(bindablePtrs));
}

std::unique_ptr<Node> Model::ParseNode(int& nextId, const aiNode& node)
{
	const auto transform = dx::XMMatrixTranspose(dx::XMLoadFloat4x4(
		reinterpret_cast<const dx::XMFLOAT4X4*>(&node.mTransformation)
	));

	std::vector<Mesh*> curMeshPtrs;
	curMeshPtrs.reserve(node.mNumMeshes);
	for (size_t i = 0; i < node.mNumMeshes; i++)
	{
		const auto meshIdx = node.mMeshes[i];
		curMeshPtrs.push_back(meshPtrs.at(meshIdx).get());
	}

	auto pNode = std::make_unique<Node>(nextId++, node.mName.C_Str(), std::move(curMeshPtrs), transform);
	for (size_t i = 0; i < node.mNumChildren; i++)
	{
		pNode->AddChild(ParseNode(nextId, *node.mChildren[i]));
	}

	return pNode;
}

void Model::Draw(Graphics& gfx) const
{
	if(auto node = pWindow->GetSelectedNode())
	{
		node->SetTransform(pWindow->GetTransform());
	}
	pRoot->Draw(gfx, dx::XMMatrixIdentity());
}

void Model::ShowWindow(Graphics& gfx, const char* windowName /*= nullptr*/)
{
	pWindow->Show(gfx, windowName, *pRoot);
}

void Model::SetRootTransform(DirectX::FXMMATRIX transform)
{
	pRoot->SetTransform(transform);
}

Model::~Model()
{}