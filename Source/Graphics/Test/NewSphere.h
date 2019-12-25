#pragma once
#include "Graphics/DX11/Graphics.h"
#include "NewCube.h"
#include "Material.h"
#include "Geometry.h"
#include "Primitive.h"

class NewSphere : public Primitive
{
public:
	NewSphere();

	virtual void InitMaterial() override;
	virtual void Update(double DeltaTime) override;
	virtual void Draw() override;
};

