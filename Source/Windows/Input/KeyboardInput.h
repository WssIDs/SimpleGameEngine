#pragma once
#include <queue>
#include <bitset>
#include <optional>


/***********************/
/* Main Class GetKeys  */
/***********************/
class KeyboardInput
{
	friend class Window;

public:
	class Event
	{

	public:
		enum class Type
		{
			Press,
			Release,
			Invalid
		};

	private:
		Type type;
		unsigned char code;

	public:
		Event(Type type, unsigned char code):
			type( type ),
			code( code ) {}

		bool IsPress() const
		{
			return type == Type::Press;
		}

		bool IsRelease() const
		{
			return type == Type::Release;
		}

		unsigned char GetCode() const
		{
			return code;
		}
	};

public:
	KeyboardInput() = default;
	KeyboardInput(const KeyboardInput&) = delete;
	KeyboardInput& operator=(const KeyboardInput&) = delete;
	// key event stuff
	bool KeyIsPressed(unsigned char keycode) const;
	std::optional<Event> ReadKey();
	bool KeyIsEmpty() const;
	void FlushKey();
	// char event stuff
	std::optional<char> ReadChar();
	bool CharIsEmpty() const;
	void FlushChar();
	void Flush();
	// autorepeat control
	void EnableAutorepeat();
	void DisableAutorepeat();
	bool AutorepeatIsEnabled() const;

private:
	void OnKeyPressed(unsigned char keycode);
	void OnKeyReleased(unsigned char keycode);
	void OnChar(char character);
	void ClearState();
	template<typename T>
	static void TrimBuffer(std::queue<T>& buffer);

private:
	static constexpr unsigned int nKeys = 256u;
	static constexpr unsigned int bufferSize = 16u;
	bool autorepeatEnabled = false;
	std::bitset<nKeys> keystates;
	std::queue<Event> keybuffer;
	std::queue<char> charbuffer;


};

template<typename T>
void KeyboardInput::TrimBuffer(std::queue<T>& buffer) 
{
	while (buffer.size() > bufferSize)
	{
		buffer.pop();
	}
}

