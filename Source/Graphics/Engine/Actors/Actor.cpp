#include "Actor.h"
#include "Windows\Launch\Window\WindowKeyMessageHandler.h"

void Actor::CreatePlayerInputComponent()
{
	if(InputSystem == nullptr)
	{
		InputSystem = std::make_shared<TestInputSystem>();
	}

	WindowKeyMessageHandler::Get()->SetInputSystem(InputSystem.get());
	SetupPlayerInputComponent(InputSystem.get());
}

void Actor::SetupPlayerInputComponent(TestInputSystem* InputComponent)
{

}

void Actor::Tick(double deltaTime)
{
	WObject::Tick(deltaTime);
}

void Actor::Render(double deltaTime)
{
	WObject::Render(deltaTime);
}

void Actor::DisableInput()
{
	InputSystem->SetBlockInput(true);
}

void Actor::EnableInput()
{
	InputSystem->SetBlockInput(false);
}

void Actor::ToggleInput()
{
	InputSystem->ToggleBlockInput();
}

bool Actor::IsBlockInput() const
{
	return InputSystem->IsBlockInput();
}
