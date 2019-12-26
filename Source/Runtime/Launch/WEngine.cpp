#include "WEngine.h"


WEngine* GEngine = nullptr;

void WEngine::Init(IEngineLoop* InEngineLoop)
{
	EngineLoop = InEngineLoop;
}