#pragma once
#include "Graphics/DX11/Graphics.h"
#include "TestNewCube.h"
#include "Material.h"
#include "Geometry.h"
#include "Primitive.h"

class TestNewSphere : public Primitive
{
public:
	TestNewSphere();

	virtual void InitMaterial() override;
	virtual void Update(double DeltaTime) override;
	virtual void Draw() override;
};

