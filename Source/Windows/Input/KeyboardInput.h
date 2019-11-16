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
		Type m_type;
		unsigned char m_code;

	public:
		Event(Type type, unsigned char code):
			m_type( type ),
			m_code( code ) {}

		bool IsPress() const
		{
			return m_type == Type::Press;
		}

		bool IsRelease() const
		{
			return m_type == Type::Release;
		}

		unsigned char GetCode() const
		{
			return m_code;
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
	static constexpr unsigned int m_nKeys = 256u;
	static constexpr unsigned int m_bufferSize = 16u;
	bool m_autorepeatEnabled = false;
	std::bitset<m_nKeys> m_keystates;
	std::queue<Event> m_keybuffer;
	std::queue<char> m_charbuffer;


};

template<typename T>
void KeyboardInput::TrimBuffer(std::queue<T>& buffer) 
{
	while (buffer.size() > m_bufferSize)
	{
		buffer.pop();
	}
}

