#include "WObject.h"

WObject::WObject()
{
}

WObject::~WObject()
{
	//delete Outer;
	Outer = nullptr;
}

void WObject::Tick(double deltaTime)
{
	for (auto& child : ChildObjects)
	{
		child->Tick(deltaTime);
	}
}

void WObject::Render(double deltaTime)
{
	for (auto& child : ChildObjects)
	{
		child->Render(deltaTime);
	}
}

std::string WObject::GetName() const
{
	return Name;
}

WObject* WObject::GetOuter() const
{
	return Outer;
}

void WObject::SetName(const std::string& name)
{
	Name = name;
}

void WObject::SetOuter(WObject* inOuter)
{
	Outer = inOuter;
}

std::vector<std::shared_ptr<WObject>> WObject::GetChildObjects()
{
	return ChildObjects;
}