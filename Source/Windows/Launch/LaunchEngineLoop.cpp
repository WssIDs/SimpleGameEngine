#include "LaunchEngineLoop.h"
#include "App.h"

FEngineLoop::FEngineLoop()
{}

int FEngineLoop::Init()
{
	GEngine = new WEngine();

	GEngine->Init(this);

	return 0;
}

void FEngineLoop::Tick()
{
	if(GEngine == nullptr)
	{
		GEngine = new WEngine();
	}

	GEngine->Tick((float)FApp::GetDeltaTime());
}
