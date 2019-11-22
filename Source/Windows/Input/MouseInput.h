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
		Type type;
		bool leftIsPressed;
		bool rightIsPressed;
		int x;
		int y;
	public:
		Event(Type type, const MouseInput& parent)
			:
			type(type),
			leftIsPressed(parent.leftIsPressed),
			rightIsPressed(parent.rightIsPressed),
			x(parent.x),
			y(parent.y)
		{}
		Type GetType() const
		{
			return type;
		}
		std::pair<int, int> GetPos() const
		{
			return{ x, y };
		}
		int GetPosX() const
		{
			return x;
		}
		int GetPosY() const
		{
			return y;
		}
		bool LeftIsPressed() const
		{
			return leftIsPressed;
		}
		bool RightIsPressed() const
		{
			return rightIsPressed;
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
		return buffer.empty();
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
	static constexpr unsigned int bufferSize = 16u;
	int x = 0;
	int y = 0;
	bool leftIsPressed = false;
	bool rightIsPressed = false;
	bool isInWindow = false;
	int wheelDeltaCarry = 0;
	bool rawEnabled = false;
	std::queue<Event> buffer;
	std::queue<RawDelta> rawDeltaBuffer;
};