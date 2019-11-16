#include "KeyboardInput.h"

bool KeyboardInput::KeyIsPressed(unsigned char keycode) const
{
	return m_keystates[keycode];
}

std::optional<KeyboardInput::Event> KeyboardInput::ReadKey()
{
	if( m_keybuffer.size() > 0u)
	{
		KeyboardInput::Event e = m_keybuffer.front();
		m_keybuffer.pop();

		return e;
	}
	
	return {};
}

bool KeyboardInput::KeyIsEmpty() const
{
	return m_keybuffer.empty();
}

void KeyboardInput::FlushKey()
{
	m_keybuffer = std::queue<Event>();
}

std::optional<char> KeyboardInput::ReadChar()
{
	if( m_charbuffer.size() > 0u )
	{
		unsigned char charcode = m_charbuffer.front();
		m_charbuffer.pop();
		return charcode;
	}
	return {};
}

bool KeyboardInput::CharIsEmpty() const
{
	return m_charbuffer.empty();
}

void KeyboardInput::FlushChar()
{
	m_charbuffer = std::queue<char>();
}

void KeyboardInput::Flush()
{
	FlushKey();
	FlushChar();
}

void KeyboardInput::EnableAutorepeat()
{
	m_autorepeatEnabled = true;
}

void KeyboardInput::DisableAutorepeat()
{
	m_autorepeatEnabled = false;
}

bool KeyboardInput::AutorepeatIsEnabled() const
{
	return m_autorepeatEnabled;
}

void KeyboardInput::OnKeyPressed(unsigned char keycode)
{
	m_keystates[keycode] = true;
	m_keybuffer.push(KeyboardInput::Event(KeyboardInput::Event::Type::Press, keycode));
	TrimBuffer(m_keybuffer);
}

void KeyboardInput::OnKeyReleased(unsigned char keycode)
{
	m_keystates[keycode] = false;
	m_keybuffer.push(KeyboardInput::Event(KeyboardInput::Event::Type::Press, keycode));
	TrimBuffer(m_keybuffer);
}

void KeyboardInput::OnChar(char character)
{
	m_charbuffer.push(character);
	TrimBuffer(m_charbuffer);
}

void KeyboardInput::ClearState()
{
	m_keystates.reset();
}
