#pragma once
#include "..\Actor.h"
#include "Graphics\DX11\Render\PointLight.h"
#include <memory>




class TestPointLight : public Actor
{
public:
	TestPointLight();
	TestPointLight(const std::string& name);

	void SetName(std::string name);

	virtual void Tick(double deltaTime) override;

	std::shared_ptr<PointLight> GetPointLight() const;

private:
	std::shared_ptr<PointLight> Light;
	std::string Name;
};

DECLARE_LOG_CATEGORY_EXTERN(TestPointLightLog)