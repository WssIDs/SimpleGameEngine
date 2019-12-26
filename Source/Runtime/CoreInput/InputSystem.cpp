#include "InputSystem.h"

DEFINE_LOG_CATEGORY(InputLog)


bool InputSystem::IsBlockInput() const
{
	return bBlockInput;
}

void InputSystem::ToggleBlockInput()
{
	bBlockInput = !bBlockInput;
}

void InputSystem::SetBlockInput(bool bNewBlockInput)
{
	bBlockInput = bNewBlockInput;
}
