#pragma once
#include "WEngine.h"


class FEngineLoop : public IEngineLoop
{
public:

	FEngineLoop();

	virtual ~FEngineLoop() {};

public:
	virtual int Init() override;
	virtual void Tick() override;

};