#include "Actor.h"
#include <Runtime/CoreInput/WindowKeyMessageHandler.h>

void Actor::CreatePlayerInputComponent()
{
	if(InputComponent == nullptr)
	{
		InputComponent = std::make_shared<InputSystem>();
	}

	WindowKeyMessageHandler::Get()->SetInputSystem(InputComponent.get());
	SetupPlayerInputComponent(InputComponent.get());
}

void Actor::SetupPlayerInputComponent(InputSystem* InputComponent)
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
	InputComponent->SetBlockInput(true);
}

void Actor::EnableInput()
{
	InputComponent->SetBlockInput(false);
}

void Actor::ToggleInput()
{
	InputComponent->ToggleBlockInput();
}

bool Actor::IsBlockInput() const
{
	return InputComponent->IsBlockInput();
}
