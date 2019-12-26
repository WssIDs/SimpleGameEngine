#pragma once
#include <Runtime/Core/Core.h>
#include <Runtime/Launch/WEngine.h>

class WEngine : public WObject
{
public:
	/** Engine loop, used for callbacks from the engine module into launch. */
	class IEngineLoop* EngineLoop = nullptr;

	virtual void Tick(float DeltaSeconds);

	/** Initialize the game engine. */
	virtual void Init(IEngineLoop* InEngineLoop);

protected:

	//std::vector<WWorld> WorldList;
};

/** Global engine pointer. Can be 0 so don't use without checking. */
extern class WEngine* GEngine;

DECLARE_LOG_CATEGORY_EXTERN(WEngineLog)