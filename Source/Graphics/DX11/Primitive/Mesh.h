#pragma once
#include "..\Drawable\DrawableBase.h"
#include "..\Bindable\BindableCommon.h"
#include "..\Render\Vertex.h"
#include <assimp\scene.h>
#include <assimp\Importer.hpp>
#include <assimp\postprocess.h>
#include "Windows\Launch\Window\STypes.h"
#include <optional>



class Mesh : public DrawableBase<Mesh>
{
public:
	Mesh(Graphics& gfx, std::vector<std::unique_ptr<Bind::Bindable>> bindPtrs);
	void Draw(Graphics& gfx, DirectX::FXMMATRIX accumulatedTransform) const;
	DirectX::XMMATRIX GetTransformXM() const override;

private:
	mutable DirectX::XMFLOAT4X4 transform;
};


class Node
{
	friend class Model;

public:
	Node(int id, const std::string& name, std::vector<Mesh*> meshPtrs, const DirectX::XMMATRIX& inTransform);
	void Draw(Graphics& gfx, DirectX::FXMMATRIX accumulatedTransform) const;
	void SetTransform(DirectX::FXMMATRIX inTransform);
	int GetId() const;
	void ShowTree(Node*& pSelectedNode) const;
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
	void ShowWindow(const char* windowName = nullptr);
	~Model();
private:
	static std::unique_ptr<Mesh> ParseMesh(Graphics& gfx, const aiMesh& mesh);
	std::unique_ptr<Node> ParseNode(int& nextId, const aiNode& node);
private:
	std::unique_ptr<Node> pRoot;
	std::vector<std::unique_ptr<Mesh>> meshPtrs;
	std::unique_ptr<class ModelWindow> pWindow;
};