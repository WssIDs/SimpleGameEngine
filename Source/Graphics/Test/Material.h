#pragma once
#include "TestTexture.h"
#include "boost\serialization\access.hpp"
#include "boost\serialization\vector.hpp"



class Material
{
public:
	Material() = default;
	Material(const std::string& EffectPath);
	void Load();
	bool IsLoaded();

	void AddTexture(const std::string& texturepath);

	Microsoft::WRL::ComPtr<ID3DBlob> VertexShaderBuffer;
	Microsoft::WRL::ComPtr<ID3DBlob> PixelShaderBuffer;

	Microsoft::WRL::ComPtr<ID3D11VertexShader> VertexShader;
	Microsoft::WRL::ComPtr<ID3D11PixelShader> PixelShader;

	std::vector<std::shared_ptr<TestTexture>> DiffuseTextures;
	std::vector<TestTexture> NormalTextures;

	std::vector<ID3D11SamplerState*> DiffuseSamplers;
	std::vector<ID3D11ShaderResourceView*> DiffuseShaderResourceViews;

	std::vector<unsigned char> VertexShaderBufferData;
	std::vector<unsigned char> PixelShaderBufferData;

	void Serialize();

	void Deserialize(const std::string& filename);

private:
	bool bLoaded;

	std::string MaterialName;
	std::string EffectPath;

	std::vector<std::string> TexturePaths;

	friend class boost::serialization::access;

	template<class Archive>
	void serialize(Archive& ar, const unsigned int version)
	{
		ar& MaterialName;
		ar& EffectPath;
		ar& TexturePaths;
		ar& VertexShaderBufferData;
		ar& PixelShaderBufferData;
	}
};

