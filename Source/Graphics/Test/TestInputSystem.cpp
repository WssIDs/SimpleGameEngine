#include "TestInputSystem.h"

DEFINE_LOG_CATEGORY(InputLog)

TestInputSystem& TestInputSystem::Get()
{
	static TestInputSystem input;
	return input;
}
