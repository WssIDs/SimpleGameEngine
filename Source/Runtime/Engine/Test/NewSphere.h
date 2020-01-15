#pragma once
#include <Runtime/Core/Core.h>
#include "Material.h"
#include "Geometry.h"
#include "Primitive.h"

class NewSphere : public Primitive
{
public:
	NewSphere();

	bool Init();
	virtual void InitMaterial() override;
	virtual void Update(double DeltaTime) override;
	virtual void Draw() override;
};

