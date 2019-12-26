#pragma once
#include <Runtime/Engine/Engine.h>

class IEngineLoop
{
public:

	virtual int Init() = 0;
	virtual void Tick() = 0;
};