#include "WindowKeyMessageHandler.h"
#include "Graphics\DX11\Math\WGMath.h"

DEFINE_LOG_CATEGORY(MessageHandlerLog)




WindowKeyMessageHandler::WindowKeyMessageHandler()
{
	AxisBindings.push_back(AxisBinding('w', EInputEvent::IE_Pressed, 1.0f));
	AxisBindings.push_back(AxisBinding('s', EInputEvent::IE_Pressed, -1.0f));
	AxisBindings.push_back(AxisBinding('a', EInputEvent::IE_Pressed, -1.0f));
	AxisBindings.push_back(AxisBinding('d', EInputEvent::IE_Pressed, 1.0f));
	AxisBindings.push_back(AxisBinding('e', EInputEvent::IE_Pressed, 1.0f));
	AxisBindings.push_back(AxisBinding('f', EInputEvent::IE_Pressed, -1.0f));
}

void WindowKeyMessageHandler::Tick(double DeltaTime)
{
	if(!AxisBindings.empty())
	{
		for (auto& axisItem : AxisBindings)
		{
			if (GetAsyncKeyState(VkKeyScan(axisItem.Key)))
			{
				axisItem.KeyEvent = EInputEvent::IE_Pressed;
				axisItem.Scale = WGMath::FInterpConstantTo(axisItem.Scale, axisItem.MaxScale, (float)DeltaTime, 7.0f);
				TestInputSystem::Get().ExecuteAxis(axisItem.Key, axisItem.KeyEvent, axisItem.Scale);
			}
			else
			{
				axisItem.KeyEvent = EInputEvent::IE_Released;
				axisItem.Scale = WGMath::FInterpConstantTo(axisItem.Scale, 0, (float)DeltaTime, 7.0f);
				TestInputSystem::Get().ExecuteAxis(axisItem.Key, axisItem.KeyEvent, axisItem.Scale);
			}
		}
	}

	if (!KeyBindings.empty())
	{
		KeyBinding bind = KeyBindings.top();
		TestInputSystem::Get().ExecuteKey(bind.Key, bind.KeyEvent);

		KeyBindings.pop();
	}
}

void WindowKeyMessageHandler::AddListener(AxisBinding AxisBind)
{
	AxisBindings.push_back(AxisBind);
}

void WindowKeyMessageHandler::AddListener(KeyBinding KeyBind)
{
	KeyBindings.push(KeyBind);
}
