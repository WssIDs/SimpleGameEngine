#pragma once
#include <queue>
#include <bitset>
#include <optional>



class MouseInput
{
	friend class Window;
public:
	struct RawDelta
	{
		int x, y;
	};
	class Event
	{
	public:
		enum class Type
		{
			LeftPress,
			LeftRelease,
			RightPress,
			RightRelease,
			WheelUp,
			WheelDown,
			Move,
			Enter,
			Leave,
		};
	private:
		Type m_type;
		bool m_leftIsPressed;
		bool m_rightIsPressed;
		int m_x;
		int m_y;
	public:
		Event(Type type, const MouseInput& parent)
			:
			m_type(type),
			m_leftIsPressed(parent.m_leftIsPressed),
			m_rightIsPressed(parent.m_rightIsPressed),
			m_x(parent.m_x),
			m_y(parent.m_y)
		{}
		Type GetType() const
		{
			return m_type;
		}
		std::pair<int, int> GetPos() const
		{
			return{ m_x, m_y };
		}
		int GetPosX() const
		{
			return m_x;
		}
		int GetPosY() const
		{
			return m_y;
		}
		bool LeftIsPressed() const
		{
			return m_leftIsPressed;
		}
		bool RightIsPressed() const
		{
			return m_rightIsPressed;
		}
	};
public:
	MouseInput() = default;
	MouseInput(const MouseInput&) = delete;
	MouseInput& operator=(const MouseInput&) = delete;
	std::pair<int, int> GetPos() const;
	std::optional<RawDelta> ReadRawDelta();
	int GetPosX() const;
	int GetPosY() const;
	bool IsInWindow() const;
	bool LeftIsPressed() const;
	bool RightIsPressed() const;
	std::optional<MouseInput::Event> Read();
	bool IsEmpty() const
	{
		return m_buffer.empty();
	}
	void Flush();
	void EnableRaw();
	void DisableRaw();
	bool RawEnabled() const;
private:
	void OnMouseMove(int x, int y) ;
	void OnMouseLeave();
	void OnMouseEnter();
	void OnRawDelta(int dx, int dy);
	void OnLeftPressed(int x, int y);
	void OnLeftReleased(int x, int y);
	void OnRightPressed(int x, int y);
	void OnRightReleased(int x, int y);
	void OnWheelUp(int x, int y);
	void OnWheelDown(int x, int y);
	void TrimBuffer();
	void TrimRawInputBuffer();
	void OnWheelDelta(int x, int y, int delta);
private:
	static constexpr unsigned int m_bufferSize = 16u;
	int m_x = 0;
	int m_y = 0;
	bool m_leftIsPressed = false;
	bool m_rightIsPressed = false;
	bool m_isInWindow = false;
	int m_wheelDeltaCarry = 0;
	bool m_rawEnabled = false;
	std::queue<Event> m_buffer;
	std::queue<RawDelta> m_rawDeltaBuffer;
};