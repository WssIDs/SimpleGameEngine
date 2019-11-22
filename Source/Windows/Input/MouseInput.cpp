#include "MouseInput.h"
#include <Windows.h>

std::pair<int, int> MouseInput::GetPos() const
{
	return { x, y };
}

std::optional<MouseInput::RawDelta> MouseInput::ReadRawDelta()
{
	if(rawDeltaBuffer.empty())
	{
		return std::nullopt;
	}

	const RawDelta delta = rawDeltaBuffer.front();
	rawDeltaBuffer.pop();

	return delta;
}

int MouseInput::GetPosX() const
{
	return x;
}

int MouseInput::GetPosY() const
{
	return y;
}

bool MouseInput::IsInWindow() const
{
	return isInWindow;
}

bool MouseInput::LeftIsPressed() const
{
	return leftIsPressed;
}

bool MouseInput::RightIsPressed() const
{
	return rightIsPressed;
}

std::optional<MouseInput::Event> MouseInput::Read()
{
	if( !buffer.empty() )
	{
		MouseInput::Event e = buffer.front();
		buffer.pop();
		return e;
	}

	return {};
}

void MouseInput::Flush()
{
	buffer = std::queue<Event>();
}

void MouseInput::EnableRaw()
{
	rawEnabled = true;
}

void MouseInput::DisableRaw()
{
	rawEnabled = false;
}

bool MouseInput::RawEnabled() const
{
	return rawEnabled;
}

void MouseInput::OnMouseMove(int x, int y)
{
	this->x = x;
	this->y = y;

	buffer.push(MouseInput::Event(MouseInput::Event::Type::Move, *this));
	TrimBuffer();
}

void MouseInput::OnMouseLeave()
{
	isInWindow = false;
	buffer.push(MouseInput::Event(MouseInput::Event::Type::Leave, *this));
	TrimBuffer();
}

void MouseInput::OnMouseEnter()
{
	isInWindow = true;
	buffer.push(MouseInput::Event(MouseInput::Event::Type::Enter, *this));
	TrimBuffer();
}

void MouseInput::OnRawDelta(int dx, int dy)
{
	rawDeltaBuffer.push({ dx,dy });
	TrimBuffer();
}

void MouseInput::OnLeftPressed(int x, int y)
{
	leftIsPressed = true;
	buffer.push(MouseInput::Event(MouseInput::Event::Type::LeftPress, *this));
	TrimBuffer();
}

void MouseInput::OnLeftReleased(int x, int y)
{
	leftIsPressed = false;
	buffer.push(MouseInput::Event(MouseInput::Event::Type::LeftRelease, *this));
	TrimBuffer();
}

void MouseInput::OnRightPressed(int x, int y)
{
	rightIsPressed = true;
	buffer.push(MouseInput::Event(MouseInput::Event::Type::RightPress, *this));
	TrimBuffer();
}

void MouseInput::OnRightReleased(int x, int y)
{
	rightIsPressed = false;
	buffer.push(MouseInput::Event(MouseInput::Event::Type::RightRelease, *this));
	TrimBuffer();
}

void MouseInput::OnWheelUp(int x, int y)
{
	buffer.push(MouseInput::Event(MouseInput::Event::Type::WheelUp, *this));
	TrimBuffer();
}

void MouseInput::OnWheelDown(int x, int y)
{
	buffer.push(MouseInput::Event(MouseInput::Event::Type::WheelDown, *this));
	TrimBuffer();
}

void MouseInput::TrimBuffer()
{
	while (buffer.size() > bufferSize)
	{
		buffer.pop();
	}
}

void MouseInput::TrimRawInputBuffer()
{
	while (rawDeltaBuffer.size() > bufferSize)
	{
		rawDeltaBuffer.pop();
	}
}

void MouseInput::OnWheelDelta(int x, int y, int delta)
{
	wheelDeltaCarry += delta;
	// generate events for every 120 
	while (wheelDeltaCarry >= WHEEL_DELTA)
	{
		wheelDeltaCarry -= WHEEL_DELTA;
		OnWheelUp(x, y);
	}
	while (wheelDeltaCarry <= -WHEEL_DELTA)
	{
		wheelDeltaCarry += WHEEL_DELTA;
		OnWheelDown(x, y);
	}
}
