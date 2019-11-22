#include "KeyboardInput.h"

bool KeyboardInput::KeyIsPressed(unsigned char keycode) const
{
	return keystates[keycode];
}

std::optional<KeyboardInput::Event> KeyboardInput::ReadKey()
{
	if( keybuffer.size() > 0u)
	{
		KeyboardInput::Event e = keybuffer.front();
		keybuffer.pop();

		return e;
	}
	
	return {};
}

bool KeyboardInput::KeyIsEmpty() const
{
	return keybuffer.empty();
}

void KeyboardInput::FlushKey()
{
	keybuffer = std::queue<Event>();
}

std::optional<char> KeyboardInput::ReadChar()
{
	if( charbuffer.size() > 0u )
	{
		unsigned char charcode = charbuffer.front();
		charbuffer.pop();
		return charcode;
	}
	return {};
}

bool KeyboardInput::CharIsEmpty() const
{
	return charbuffer.empty();
}

void KeyboardInput::FlushChar()
{
	charbuffer = std::queue<char>();
}

void KeyboardInput::Flush()
{
	FlushKey();
	FlushChar();
}

void KeyboardInput::EnableAutorepeat()
{
	autorepeatEnabled = true;
}

void KeyboardInput::DisableAutorepeat()
{
	autorepeatEnabled = false;
}

bool KeyboardInput::AutorepeatIsEnabled() const
{
	return autorepeatEnabled;
}

void KeyboardInput::OnKeyPressed(unsigned char keycode)
{
	keystates[keycode] = true;
	keybuffer.push(KeyboardInput::Event(KeyboardInput::Event::Type::Press, keycode));
	TrimBuffer(keybuffer);
}

void KeyboardInput::OnKeyReleased(unsigned char keycode)
{
	keystates[keycode] = false;
	keybuffer.push(KeyboardInput::Event(KeyboardInput::Event::Type::Press, keycode));
	TrimBuffer(keybuffer);
}

void KeyboardInput::OnChar(char character)
{
	charbuffer.push(character);
	TrimBuffer(charbuffer);
}

void KeyboardInput::ClearState()
{
	keystates.reset();
}
