#include "Mesh.h"
#include "Imgui\imgui.h"
#include <unordered_map>
#include "assimp\material.h"
#include "..\Bindable\Texture.h"
#include "..\Bindable\Sampler.h"
#include "..\Render\Surface.h"
#include "Windows\Logger\LogDefinitions.h"

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
	void Show(const char* windowName, const Node& root) noexcept
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
	Node* pSelectedNode;
	struct TransformParameters
	{
		float roll = 0.0f;
		float pitch = 0.0f;
		float yaw = 0.0f;
		float x = 0.0f;
		float y = 0.0f;
		float z = 0.0f;
	};

	std::unordered_map<int, TransformParameters> transforms;
};


// Model
Model::Model(Graphics& gfx, const std::string fileName)
	:
	pWindow(std::make_unique<ModelWindow>())
{
	Assimp::Importer imp;
	const auto pScene = imp.ReadFile(fileName.c_str(),
		aiProcess_Triangulate |
		aiProcess_JoinIdenticalVertices |
		aiProcess_ConvertToLeftHanded |
		aiProcess_GenNormals |
		aiProcess_CalcTangentSpace
	);

	for (size_t i = 0; i < pScene->mNumMeshes; i++)
	{
		meshPtrs.push_back(ParseMesh(gfx, *pScene->mMeshes[i], pScene->mMaterials));
	}

	int nextId = 0;
	pRoot = ParseNode(nextId, *pScene->mRootNode);
}

std::unique_ptr<Mesh> Model::ParseMesh(Graphics& gfx, const aiMesh& mesh, const aiMaterial* const* pMaterials)
{
	using DynamicVtx::VertexLayout;

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
			*reinterpret_cast<dx::XMFLOAT3*>(&mesh.mVertices[i]),
			*reinterpret_cast<dx::XMFLOAT3*>(&mesh.mNormals[i]),
			*reinterpret_cast<dx::XMFLOAT3*>(&mesh.mTangents[i]),
			*reinterpret_cast<dx::XMFLOAT3*>(&mesh.mBitangents[i]),
			*reinterpret_cast<dx::XMFLOAT2*>(&mesh.mTextureCoords[0][i])
		);
	}

	std::vector<unsigned short> indices;
	indices.reserve(mesh.mNumFaces * 3);
	for (unsigned int i = 0; i < mesh.mNumFaces; i++)
	{
		const auto& face = mesh.mFaces[i];
		assert(face.mNumIndices == 3);
		indices.push_back(face.mIndices[0]);
		indices.push_back(face.mIndices[1]);
		indices.push_back(face.mIndices[2]);
	}

	std::vector<std::shared_ptr<Bind::Bindable>> bindablePtrs;
	bool bSpecularMap = false;
	
	float shininess = 35.0f;
	if (mesh.mMaterialIndex >= 0)
	{
		auto& material = *pMaterials[mesh.mMaterialIndex];
		aiString texFileName;
		material.GetTexture(aiTextureType_DIFFUSE, 0, &texFileName);
		bindablePtrs.push_back(Bind::Texture::Resolve(gfx, BASE_MODELS_DIR + texFileName.C_Str()));

		if (material.GetTexture(aiTextureType_SPECULAR, 0, &texFileName) == aiReturn_SUCCESS)
		{
			bindablePtrs.push_back(Bind::Texture::Resolve(gfx, BASE_MODELS_DIR + texFileName.C_Str(), 1));
			bSpecularMap = true;
		}
		else
		{
			material.Get(AI_MATKEY_SHININESS, shininess);
		}

		material.GetTexture(aiTextureType_NORMALS, 0, &texFileName);
		bindablePtrs.push_back(Bind::Texture::Resolve(gfx, BASE_MODELS_DIR + texFileName.C_Str(), 2));

		if (material.GetTexture(aiTextureType_SHININESS, 0, &texFileName) == aiReturn_SUCCESS)
		{
			bindablePtrs.push_back(Bind::Texture::Resolve(gfx, BASE_MODELS_DIR + texFileName.C_Str(), 3));
		}

		bindablePtrs.push_back(Bind::Sampler::Resolve(gfx));
	}

	auto meshTag = BASE_MODELS_DIR + "%" + mesh.mName.C_Str();

	bindablePtrs.push_back(Bind::VertexBuffer::Resolve(gfx, meshTag, vertexBuffer));

	bindablePtrs.push_back(Bind::IndexBuffer::Resolve(gfx, meshTag, indices));

	auto pvs = Bind::VertexShader::Resolve(gfx, BASE_SHADERS_DIR + "PhongVSNormalMap.cso");
	auto pvsbc = pvs->GetByteCode();
	bindablePtrs.push_back(std::move(pvs));

	bindablePtrs.push_back(Bind::InputLayout::Resolve(gfx, vertexBuffer.GetLayout(), pvsbc));

	if(bSpecularMap)
	{
		bindablePtrs.push_back(Bind::PixelShader::Resolve(gfx, BASE_SHADERS_DIR + "PhongPSSpecularNormalMap.cso"));
	}
	else
	{
		bindablePtrs.push_back(Bind::PixelShader::Resolve(gfx, BASE_SHADERS_DIR +  "PhongPSNormalMap.cso"));

		struct PSMaterialConstant
		{
			float specularIntensity = 0.18f;
			float specularPower;
			float padding[2];
		} pmc;

		pmc.specularPower = shininess;

		bindablePtrs.push_back(Bind::PixelConstantBuffer<PSMaterialConstant>::Resolve(gfx, pmc, 1u));
	}

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

void Model::ShowWindow(const char* windowName /*= nullptr*/)
{
	pWindow->Show(windowName, *pRoot);
}

void Model::SetRootTransform(DirectX::FXMMATRIX transform)
{
	pRoot->SetTransform(transform);
}

Model::~Model()
{}