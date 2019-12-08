#pragma once
#include "..\Test\WObject.h"



class Canvas : public WObject
{
public:

	virtual void Tick(double deltaTime) override;
	virtual void Render(double deltaTime) override;

};

