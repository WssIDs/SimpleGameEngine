#include "MouseInput.h"
#include <Windows.h>

std::pair<int, int> MouseInput::GetPos() const
{
	return { m_x, m_y };
}

std::optional<MouseInput::RawDelta> MouseInput::ReadRawDelta()
{
	if(m_rawDeltaBuffer.empty())
	{
		return std::nullopt;
	}

	const RawDelta delta = m_rawDeltaBuffer.front();
	m_rawDeltaBuffer.pop();

	return delta;
}

int MouseInput::GetPosX() const
{
	return m_x;
}

int MouseInput::GetPosY() const
{
	return m_y;
}

bool MouseInput::IsInWindow() const
{
	return m_isInWindow;
}

bool MouseInput::LeftIsPressed() const
{
	return m_leftIsPressed;
}

bool MouseInput::RightIsPressed() const
{
	return m_rightIsPressed;
}

std::optional<MouseInput::Event> MouseInput::Read()
{
	if( m_buffer.size() > 0u )
	{
		MouseInput::Event e = m_buffer.front();
		m_buffer.pop();
		return e;
	}

	return {};
}

void MouseInput::Flush()
{
	m_buffer = std::queue<Event>();
}

void MouseInput::EnableRaw()
{
	m_rawEnabled = true;
}

void MouseInput::DisableRaw()
{
	m_rawEnabled = false;
}

bool MouseInput::RawEnabled() const
{
	return m_rawEnabled;
}

void MouseInput::OnMouseMove(int x, int y)
{
	m_x = x;
	m_y = y;

	m_buffer.push(MouseInput::Event(MouseInput::Event::Type::Move, *this));
	TrimBuffer();
}

void MouseInput::OnMouseLeave()
{
	m_isInWindow = false;
	m_buffer.push(MouseInput::Event(MouseInput::Event::Type::Leave, *this));
	TrimBuffer();
}

void MouseInput::OnMouseEnter()
{
	m_isInWindow = true;
	m_buffer.push(MouseInput::Event(MouseInput::Event::Type::Enter, *this));
	TrimBuffer();
}

void MouseInput::OnRawDelta(int dx, int dy)
{
	m_rawDeltaBuffer.push({ dx,dy });
	TrimBuffer();
}

void MouseInput::OnLeftPressed(int x, int y)
{
	m_leftIsPressed = true;
	m_buffer.push(MouseInput::Event(MouseInput::Event::Type::LeftPress, *this));
	TrimBuffer();
}

void MouseInput::OnLeftReleased(int x, int y)
{
	m_leftIsPressed = false;
	m_buffer.push(MouseInput::Event(MouseInput::Event::Type::LeftRelease, *this));
	TrimBuffer();
}

void MouseInput::OnRightPressed(int x, int y)
{
	m_rightIsPressed = true;
	m_buffer.push(MouseInput::Event(MouseInput::Event::Type::RightPress, *this));
	TrimBuffer();
}

void MouseInput::OnRightReleased(int x, int y)
{
	m_rightIsPressed = false;
	m_buffer.push(MouseInput::Event(MouseInput::Event::Type::RightRelease, *this));
	TrimBuffer();
}

void MouseInput::OnWheelUp(int x, int y)
{
	m_buffer.push(MouseInput::Event(MouseInput::Event::Type::WheelUp, *this));
	TrimBuffer();
}

void MouseInput::OnWheelDown(int x, int y)
{
	m_buffer.push(MouseInput::Event(MouseInput::Event::Type::WheelDown, *this));
	TrimBuffer();
}

void MouseInput::TrimBuffer()
{
	while (m_buffer.size() > m_bufferSize)
	{
		m_buffer.pop();
	}
}

void MouseInput::TrimRawInputBuffer()
{
	while (m_rawDeltaBuffer.size() > m_bufferSize)
	{
		m_rawDeltaBuffer.pop();
	}
}

void MouseInput::OnWheelDelta(int x, int y, int delta)
{
	m_wheelDeltaCarry += delta;
	// generate events for every 120 
	while (m_wheelDeltaCarry >= WHEEL_DELTA)
	{
		m_wheelDeltaCarry -= WHEEL_DELTA;
		OnWheelUp(x, y);
	}
	while (m_wheelDeltaCarry <= -WHEEL_DELTA)
	{
		m_wheelDeltaCarry += WHEEL_DELTA;
		OnWheelDown(x, y);
	}
}
