#pragma once
#include <Runtime/Core/Core.h>
#include "Primitive.h"
#include <unordered_set>

class NewMesh : public Primitive
{
public:
	NewMesh();

	bool Init();

	bool LoadMesh();
	virtual void InitMaterial() override;
	virtual void Update(double DeltaTime) override;
	virtual void Draw() override;

	void SetMesh(const std::string& FileName);

	std::vector<Vector> Vertices;
	std::vector<LinearColor> Colors;
	std::vector<Vector2D> UVs;
	std::vector<Vector> Normals;
	std::vector<Vector> Tangents;

	std::vector<int32> Indices;


	std::string MeshName;
};

DECLARE_LOG_CATEGORY_EXTERN(NewMeshLog)

