#pragma once
#include "..\Drawable\Drawable.h"
#include "..\Bindable\BindableCommon.h"
#include "..\Render\Vertex.h"
#include <assimp\scene.h>
#include <assimp\Importer.hpp>
#include <assimp\postprocess.h>
#include <optional>
#include "Graphics/Engine/Core.h"



class Mesh : public Drawable
{
public:
	Mesh(Graphics& gfx, std::vector<std::shared_ptr<Bind::Bindable>> bindPtrs);
	void Draw(Graphics& gfx, DirectX::FXMMATRIX accumulatedTransform) const;
	DirectX::XMMATRIX GetTransformXM() const override;

private:
	mutable DirectX::XMFLOAT4X4 transform;
};


class Node
{
	friend class Model;
public:

	struct PSMaterialConstantFull
	{
		BOOL normalMapEnabled = TRUE;
		BOOL specularMapEnabled = TRUE;
		BOOL specularMapAlpha = FALSE;
		BOOL glossinesMapEnabled = TRUE;
		float specularPower = 3.1f;
		DirectX::XMFLOAT3 specularColor = { 0.75f,0.75f,0.75f };
		float specularWeight = 0.67f;
		float padding[3];
	};

public:
	Node(int id, const std::string& name, std::vector<Mesh*> meshPtrs, const DirectX::XMMATRIX& inTransform);
	void Draw(Graphics& gfx, DirectX::FXMMATRIX accumulatedTransform) const;
	void SetTransform(DirectX::FXMMATRIX inTransform);
	int GetId() const;
	void ShowTree(Node*& pSelectedNode) const;
	void ControlMaterial(Graphics& gfx, PSMaterialConstantFull& c);
private:
	void AddChild(std::unique_ptr<Node> pChild);

private:
	std::string name;
	int id;
	std::vector<std::unique_ptr<Node>> childPtrs;
	std::vector<Mesh*> meshPtrs;
	DirectX::XMFLOAT4X4 transform;
	DirectX::XMFLOAT4X4 appliedTransform;
};


class Model
{
public:
	Model(Graphics& gfx, const std::string fileName);
	void Draw(Graphics& gfx) const;
	void ShowWindow(Graphics& gfx, const char* windowName = nullptr);
	void SetRootTransform(DirectX::FXMMATRIX transform);
	~Model();
private:
	static std::unique_ptr<Mesh> ParseMesh(Graphics& gfx, const aiMesh& mesh, const aiMaterial* const* pMaterials);
	std::unique_ptr<Node> ParseNode(int& nextId, const aiNode& node);
private:
	std::unique_ptr<Node> pRoot;
	std::vector<std::unique_ptr<Mesh>> meshPtrs;
	std::unique_ptr<class ModelWindow> pWindow;
};


DECLARE_LOG_CATEGORY_EXTERN(MeshLog)