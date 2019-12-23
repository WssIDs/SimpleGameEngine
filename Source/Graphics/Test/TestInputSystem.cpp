#include "TestInputSystem.h"

DEFINE_LOG_CATEGORY(InputLog)


bool TestInputSystem::IsBlockInput() const
{
	return bBlockInput;
}

void TestInputSystem::ToggleBlockInput()
{
	bBlockInput = !bBlockInput;
}

void TestInputSystem::SetBlockInput(bool bNewBlockInput)
{
	bBlockInput = bNewBlockInput;
}
