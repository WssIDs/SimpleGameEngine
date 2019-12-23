#pragma once
#include <Graphics/Engine/Engine.h>

class IEngineLoop
{
public:

	virtual int Init() = 0;
	virtual void Tick() = 0;
};