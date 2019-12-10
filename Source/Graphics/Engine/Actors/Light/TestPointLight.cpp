#include "TestPointLight.h"

DEFINE_LOG_CATEGORY(TestPointLightLog)


TestPointLight::TestPointLight()
{
	Light = NewObject<PointLight>(this, "DefaultLight");

	//Light->SetName("PointLightMain");

	//ChildObjects.push_back(Light);
}

TestPointLight::TestPointLight(const std::string& name)
{
	Light = std::make_shared<PointLight>(name, 2.0f);
	//ChildObjects.push_back(Light);
}

void TestPointLight::SetName(std::string name)
{
	Name = name;
}

void TestPointLight::Tick(double deltaTime)
{
	Actor::Tick(deltaTime);

	//WGE_LOG(TestPointLightLog, LogVerbosity::Default, "Light pos = x=%lf|y=%lf|z=%lf",Light->GetPosition().x, Light->GetPosition().y, Light->GetPosition().z);
	//WGE_LOG(TestPointLightLog, LogVerbosity::Default, "LightFromChildData pos = x=%lf|y=%lf|z=%lf", 
	//	std::static_pointer_cast<PointLight>(GetChildObjects()[0])->GetPosition().x,
	//	std::static_pointer_cast<PointLight>(GetChildObjects()[0])->GetPosition().y,
	//	std::static_pointer_cast<PointLight>(GetChildObjects()[0])->GetPosition().z);
}

std::shared_ptr<PointLight> TestPointLight::GetPointLight() const
{
	return Light;
}
