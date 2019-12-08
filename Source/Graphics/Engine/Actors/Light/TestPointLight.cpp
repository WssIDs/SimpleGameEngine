#include "TestPointLight.h"

TestPointLight::TestPointLight()
{
	Light = std::make_shared<PointLight>("Default Point Light",2.0f);
	ChildObjects.push_back(Light);
}

TestPointLight::TestPointLight(const std::string& name)
{
	Light = std::make_shared<PointLight>(name, 2.0f);
	ChildObjects.push_back(Light);
}

void TestPointLight::SetName(std::string name)
{
	Name = name;
}

void TestPointLight::Tick(double deltaTime)
{
	Actor::Tick(deltaTime);
}

std::shared_ptr<PointLight> TestPointLight::GetPointLight() const
{
	return Light;
}
