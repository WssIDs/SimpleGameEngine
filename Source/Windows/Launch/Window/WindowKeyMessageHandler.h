#pragma once
#include <stack>
#include "Graphics\Test\TestInputSystem.h"
#include "Graphics\Engine\Core.h"
#include <set>
#include <vector>


class WindowKeyMessageHandler
{
public:
	WindowKeyMessageHandler();

	void Tick(double DeltaTime);

	void AddListener(AxisBinding AxisBind);
	void AddListener(KeyBinding KeyBind);

	std::vector<AxisBinding> AxisRawBindings;
	std::vector<AxisBinding> AxisBindings;
	std::stack<KeyBinding> KeyBindings;

	float step = 0.05f;
};

DECLARE_LOG_CATEGORY_EXTERN(MessageHandlerLog)