#include "Actor.h"

void Actor::Tick(double deltaTime)
{
	WObject::Tick(deltaTime);
}

void Actor::Render(double deltaTime)
{
	WObject::Render(deltaTime);
}

std::vector<std::shared_ptr<WObject>> Actor::GetChildObjects()
{
	return ChildObjects;
}
