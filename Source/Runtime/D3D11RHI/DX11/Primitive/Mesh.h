#pragma once
#include <Runtime/D3D11RHI/DX11/Drawable/Drawable.h>
#include <Runtime/D3D11RHI/DX11/Bindable/BindableCommon.h>
#include <optional>
#include <type_traits>
#include <boost/serialization/export.hpp>

struct aiMesh;
struct aiMaterial;
struct aiNode;


class Mesh : public Drawable
{
public:
	Mesh(DX11RHI& gfx, std::vector<std::shared_ptr<Bind::Bindable>> bindPtrs);
	void Draw(DX11RHI& gfx, DirectX::FXMMATRIX accumulatedTransform) const;
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

	struct PSMaterialConstantNotex
	{
		DirectX::XMFLOAT4 materialColor = { 0.447970f,0.327254f,0.176283f,1.0f };
		DirectX::XMFLOAT4 specularColor = { 0.65f,0.65f,0.65f,1.0f };
		float specularPower = 120.0f;
		float padding[3];
	} pmc;

public:
	Node(int id, const std::string& name, std::vector<Mesh*> meshPtrs, const DirectX::XMMATRIX& inTransform);
	void Draw(DX11RHI& gfx, DirectX::FXMMATRIX accumulatedTransform) const;
	void SetTransform(DirectX::FXMMATRIX inTransform);
	const DirectX::XMFLOAT4X4 GetAppliedTransform() const;
	int GetId() const;
	void ShowTree(Node*& pSelectedNode) const;

	template<class T>
	bool ControlMaterial(DX11RHI& gfx, T& c)
	{
		if (meshPtrs.empty())
		{
			return false;
		}

		auto mesh = meshPtrs.front();

		if constexpr (std::is_same<T, PSMaterialConstantFull>::value)
		{
			if (auto pcb = mesh->QueryBindable<Bind::PixelConstantBuffer<T>>())
			{
				ImGui::Text("Material");

				auto normalMapEnabled = (bool)c.normalMapEnabled;
				ImGui::Checkbox("Normal Map", &normalMapEnabled);
				c.normalMapEnabled = normalMapEnabled ? TRUE : FALSE;

				auto specularMapEnabled = (bool)c.specularMapEnabled;
				ImGui::Checkbox("Specular Map", &specularMapEnabled);
				c.specularMapEnabled = specularMapEnabled ? TRUE : FALSE;

				//bool specularMapAlpha = (bool)c.specularMapAlpha;
				//ImGui::Checkbox("Specular Map Alpha", &specularMapAlpha);
				//c.specularMapAlpha = specularMapAlpha ? true : false;

				auto glossinesMapEnabled = (bool)c.glossinesMapEnabled;
				ImGui::Checkbox("Glossiness Map", &glossinesMapEnabled);
				c.glossinesMapEnabled = glossinesMapEnabled ? TRUE : FALSE;

				ImGui::SliderFloat("Specular Weight", &c.specularWeight, 0.0f, 2.0f);
				ImGui::SliderFloat("Specular Power", &c.specularPower, 0.0f, 1000.0f, "%f", 5.0f);
				ImGui::ColorPicker3("Specular Color", reinterpret_cast<float*>(&c.specularColor));

				pcb->Update(gfx, c);
				return true;
			}
		}
		else if constexpr (std::is_same<T, PSMaterialConstantNotex>::value)
		{
			if (auto pcb = mesh->QueryBindable<Bind::PixelConstantBuffer<T>>())
			{
				ImGui::Text("Material");

				ImGui::ColorPicker3("Diffuse Color", reinterpret_cast<float*>(&c.materialColor));
				ImGui::ColorPicker3("Specular Color", reinterpret_cast<float*>(&c.specularColor));
				ImGui::SliderFloat("Specular Power", &c.specularPower, 0.0f, 1000.0f, "%f", 5.0f);

				pcb->Update(gfx, c);
				return true;
			}
		}

		return false;
	}

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


class Model : public WObject
{
public:
	Model() = default;
	Model(const std::string path, DirectX::XMFLOAT3 scale3D = {1.0f,1.0f,1.0f});
	void Draw() const;
	void ShowWindow(DX11RHI& gfx, const char* windowName = nullptr);
	void SetRootTransform(DirectX::FXMMATRIX transform);
	~Model();

	void Tick(double deltaTime) override;
	void Render(double deltaTime) override;

private:
	friend class boost::serialization::access;

	template<class Archive>
	void serialize(Archive& ar, const unsigned int version)
	{
		boost::serialization::base_object<IWObject>(*this);
		ar& modelName;
	}

	std::string modelName = "Sponza";

	static std::unique_ptr<Mesh> ParseMesh(const aiMesh& mesh, const aiMaterial* const* pMaterials, DirectX::XMFLOAT3 scale3D);
	std::unique_ptr<Node> ParseNode(int& nextId, const aiNode& node);
private:
	std::unique_ptr<Node> pRoot;
	std::vector<std::unique_ptr<Mesh>> meshPtrs;
	std::unique_ptr<class ModelWindow> pWindow;
};


BOOST_SERIALIZATION_ASSUME_ABSTRACT(IWObject)

DECLARE_LOG_CATEGORY_EXTERN(MeshLog)