#include "WEngine.h"


DEFINE_LOG_CATEGORY(WEngineLog)

WEngine* GEngine = nullptr;

void WEngine::Init(IEngineLoop* InEngineLoop)
{
	EngineLoop = InEngineLoop;
}

void WEngine::Tick(float DeltaSeconds)
{
	//WGE_LOG(WEngineLog, LogVerbosity::Default, "DeltaSecond = %lf", DeltaSeconds);
}