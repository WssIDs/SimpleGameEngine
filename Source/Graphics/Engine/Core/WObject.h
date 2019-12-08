#pragma once
#include "IWObject.h"

class Graphics;

class WObject : public IWObject
{
public:

	WObject();

	virtual void Tick(double deltaTime) override;
	virtual void Render(double deltaTime) override;

private:
};

