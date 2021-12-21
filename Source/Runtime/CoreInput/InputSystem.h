#pragma once
#include <Runtime/Core/Core.h>
#include <Runtime/CoreInput/InputTypes.h>
#include <vector>
#include <string>
#include <cassert>
#include <map>
#include <unordered_map>
#include <memory>


enum class EInputEvent
{
	IE_Pressed		=0,
	IE_Released		=1,
	IE_Repeat		=2,
	IE_DoubleClick	=3,
	IE_Axis			=4,
	IE_MAX			=5,
};

//   онтейнер дл€ хранени€ до 2-х аргументов.
struct NIL {};

class IArguments{
public:
	virtual ~IArguments() {}
};

template< class T1 = NIL, class T2 = NIL >
class Arguments : public IArguments
{
public:
	Arguments() {}
public:
	Arguments(T1 i_arg1) 
		:
		arg1(i_arg1)
	{}
public:
	Arguments(T1 i_arg1, T2 i_arg2)
		:
		arg1(i_arg1),
		arg2(i_arg2)
	{}
public:
	T1 arg1;
	T2 arg2;
};


struct KeyBinding
{
	KeyBinding()
		:
		ActionName(""),
		Key(EKeys::Invalid),
		KeyEvent(EInputEvent::IE_MAX)
	{}

	KeyBinding(FKey Key, EInputEvent KeyEvent)
		:
		ActionName(""),
		Key(Key),
		KeyEvent(KeyEvent)
	{
	}

	//KeyBinding(std::string ActionName, unsigned char Key)
	//	:
	//	ActionName(ActionName),
	//	Key(Key),
	//	KeyEvent(EInputEvent::IE_MAX),
	//	bAlt(false),
	//	bShift(false),
	//	bCtrl(false)
	//{
	//}

	//KeyBinding(std::string ActionName, unsigned char Key, bool bShift = false, bool bAlt = false, bool bCtrl = false)
	//	:
	//	ActionName(ActionName),
	//	Key(Key),
	//	KeyEvent(EInputEvent::IE_MAX),
	//	bShift(bShift),
	//	bAlt(bAlt),
	//	bCtrl(bCtrl)
	//{
	//}

	KeyBinding(std::string ActionName, FKey Key, bool bShift = false, bool bAlt = false, bool bCtrl = false, bool bCmd = false)
		:
		ActionName(ActionName),
		Key(Key),
		KeyEvent(EInputEvent::IE_Released),
		bShift(bShift),
		bAlt(bAlt),
		bCtrl(bCtrl),
		bCmd(bCmd)
	{
	}

	KeyBinding(std::string ActionName, EInputEvent KeyEvent)
		:
		ActionName(ActionName),
		Key(EKeys::Invalid),
		KeyEvent(KeyEvent)
	{
	}

	bool IsShiftDown() const
	{
		return bShift;
	}

	bool IsAltDown() const
	{
		return bAlt;
	}

	bool IsControlDown() const
	{
		return bCtrl;
	}

	bool IsCommandDown() const
	{
		return bCmd;
	}

	bool KeyIsDown() const
	{
		return KeyEvent == EInputEvent::IE_Pressed ? true : false;
	}

	bool KeyIsUp() const
	{
		return KeyEvent == EInputEvent::IE_Released ? true : false;
	}

	FKey GetKey() const
	{
		return Key;
	}

	bool bShift = false;
	bool bAlt = false;
	bool bCtrl = false;
	bool bCmd = false;

	std::string ActionName;
	EInputEvent KeyEvent;

	FKey Key;
};

struct AxisBinding
{
	AxisBinding()
		:
		Scale(0.0f),
		MaxScale(0.0f),
		Key(EKeys::Invalid),
		KeyEvent(EInputEvent::IE_Axis)
	{}

	AxisBinding(FKey Key, EInputEvent KeyEvent)
		:
		Scale(0.0f),
		MaxScale(0.0f),
		Key(Key),
		KeyEvent(KeyEvent)
	{}

	AxisBinding(FKey Key, float MaxScale)
		:
		Scale(0.0f),
		MaxScale(MaxScale),
		Key(Key),
		KeyEvent(EInputEvent::IE_Axis)
	{
	}

	AxisBinding(FKey Key, EInputEvent KeyEvent, float MaxScale)
		:
		Scale(0.0f),
		MaxScale(MaxScale),
		Key(Key),
		KeyEvent(KeyEvent)
	{
	}

	// for Binding Axis
	AxisBinding(const std::string& AxisName)
		:
		AxisName(AxisName),
		Scale(0.0f),
		MaxScale(0.0f),
		Key(EKeys::Invalid),
		KeyEvent(EInputEvent::IE_Axis)
	{
	}

	// For Message Handler
	AxisBinding(const std::string& AxisName, float MaxScale, FKey Key)
		:
		AxisName(AxisName),
		Scale(0.0f),
		MaxScale(MaxScale),
		Key(Key),
		KeyEvent(EInputEvent::IE_Axis)
	{
	}

	//AxisBinding(const std::string& AxisName, float MaxScale, FKey Key)
	//	:
	//	AxisName(AxisName),
	//	Scale(0.0f),
	//	MaxScale(MaxScale),
	//	Key(Key),
	//	KeyEvent(EInputEvent::IE_Axis)
	//{
	//}

	// For Message Handler
	AxisBinding(const std::string& AxisName, float MaxScale)
		:
		AxisName(AxisName),
		Scale(0.0f),
		MaxScale(MaxScale),
		Key(EKeys::Invalid),
		KeyEvent(EInputEvent::IE_Axis)
	{
	}

	void AddScale(float delta)
	{
		if (Scale < MaxScale)
		{
			Scale += delta;
		}
		else
		{
			Scale = 0.0f;
		}
	}

	FKey GetKey() const
	{
		return Key;
	}

	std::string AxisName;
	float MaxScale = 0.0f;
	float Scale = 0.0f;
	EInputEvent KeyEvent;

	FKey Key;
};

//   онтейнер дл€ хранени€ указател€ на метод.
class IContainer
{
public:
	virtual void Execute(IArguments*) = 0;
};

template< class UserClass, class Func>
class Container : public IContainer {};

//  —пециализаци€ дл€ метода без аргументов.
template< class UserClass>
class Container< UserClass, void (UserClass::*)(void) > : public IContainer
{
	typedef void (UserClass::*Func)(void);

public: 
	Container(UserClass* Object, Func Method)
		:
		Object(Object),
		Method(Method)
	{}
private:
	UserClass* Object;
	Func Method;
public:
	void Execute(IArguments* Args) override
	{
		(Object->*Method)();
	}
};

//  —пециализаци€ дл€ метода с одним аргументом.
template< class UserClass, class Arg >
class Container< UserClass, void (UserClass::*)(Arg) > : public IContainer
{
	typedef void (UserClass::* Func)(Arg);
	typedef Arguments<Arg> Arg;

public:
	Container(UserClass* Object, Func Method)
		: 
		Object(Object), 
		Method(Method)
	{}
private:
	UserClass* Object;
	Func Method;

public: void Execute(IArguments* Args) override
{
	Arg* Argument = dynamic_cast<Arg*>(Args);
	assert(Argument);
	if (Argument) (Object->*Method)(Argument->arg1);
}
};

//  —пециализаци€ дл€ метода с двум€ аргументами
//template< class T, class A1, class A2 >
//class Container< T, void (T::*)(A1, A2) > : public IContainer
//{
//	typedef void (T::* M)(A1, A2);
//	typedef Arguments<A1, A2> A;
//
//public:
//	Container(std::string& actionName, T* c, M m)
//		:
//		m_actionName(actionName),
//		m_class(c),
//		m_method(m)
//	{}
//private:
//	std::string m_actionName;
//	T* m_class;
//	M m_method;
//public:
//	void Call(IArguments* i_args)
//	{
//		A* a = dynamic_cast<A*>(i_args);
//		assert(a);
//		if (a)
//		{
//			(m_class->*m_method)(a->arg1, a->arg2);
//		}
//	}
//};


class DelegateNoArgs
{
public:

	DelegateNoArgs()
		:
		Binding(),
		DelegateContainer(nullptr)
	{}

	~DelegateNoArgs()
	{
		if (DelegateContainer)
		{
			delete DelegateContainer;
		}
	}

	template< class UserClass, class Func>
	void Bind(const FKey Key, const EInputEvent KeyEvent, UserClass* Object, Func Method)
	{
		if(DelegateContainer)
		{
			delete DelegateContainer;
		}
		
		Binding = KeyBinding(Key, KeyEvent);
		DelegateContainer = new Container<UserClass, Func>(Object, Method);
	}

	template< class UserClass, class Func>
	void Bind(const std::string& ActionName, const EInputEvent KeyEvent, UserClass* Object, Func Method)
	{
		if (DelegateContainer)
		{
			delete DelegateContainer;
		}

		Binding = KeyBinding(ActionName, KeyEvent);
		DelegateContainer = new Container<UserClass, Func>(Object, Method);
	}

	void Execute()
	{
		IArguments* arg = new Arguments();
		DelegateContainer->Execute(arg);
	}

	//template< class T1 > void operator()(T1 i_arg1)
	//{
	//	m_container->Call(&Arguments< T1 >(i_arg1));
	//}

	//template< class T1, class T2 > void operator()(T1 i_arg1, T2 i_arg2)
	//{
	//	m_container->Call(&Arguments< T1, T2 >(i_arg1, i_arg2));
	//}

	std::string GetActionName() const
	{
		return Binding.ActionName;
	}

	EInputEvent GetKeyEvent() const
	{
		return Binding.KeyEvent;
	}

	KeyBinding GetKeyInfo() const
	{
		return Binding;
	}

private:
	KeyBinding Binding;
	IContainer* DelegateContainer;
};

class DelegateWithOneArg
{
public:

	DelegateWithOneArg()
		:
		Binding(),
		DelegateContainer(nullptr)
	{}

	~DelegateWithOneArg()
	{
		if (DelegateContainer)
		{
			delete DelegateContainer;
		}
	}

	template< class UserClass, class Func>
	void Bind(const FKey Key, const EInputEvent KeyEvent, float MaxScale, UserClass* Object, Func Method)
	{
		if (DelegateContainer)
		{
			delete DelegateContainer;
		}

		Binding = AxisBinding(Key, KeyEvent, MaxScale);
		DelegateContainer = new Container<UserClass, Func>(Object, Method);
	}

	template< class UserClass, class Func>
	void Bind(const std::string& AxisName, UserClass* Object, Func Method)
	{
		if (DelegateContainer)
		{
			delete DelegateContainer;
		}

		Binding = AxisBinding(AxisName);
		DelegateContainer = new Container<UserClass, Func>(Object, Method);
	}

	template<class Arg>
	void Execute(Arg Args)
	{
		DelegateContainer->Execute(new Arguments<Arg>(Args));
	}

	//template< class T1 > void operator()(T1 i_arg1)
	//{
	//	m_container->Call(&Arguments< T1 >(i_arg1));
	//}

	//template< class T1, class T2 > void operator()(T1 i_arg1, T2 i_arg2)
	//{
	//	m_container->Call(&Arguments< T1, T2 >(i_arg1, i_arg2));
	//}

	std::string GetAxisName() const
	{
		return Binding.AxisName;
	}

	AxisBinding GetAxisInfo() const
	{
		return Binding;
	}

private:
	AxisBinding Binding;
	IContainer* DelegateContainer;
};

class InputSystem
{
public:
	InputSystem() {};
	InputSystem(const InputSystem&) = delete;
	InputSystem& operator=(const InputSystem&) = delete;

	//template< class T, class U>
	//void BindAction(const char& key, const std::string& ActionName, T* wObject, U method)
	//{
	//	auto BindIt = Binds.find(ActionName);

	//	if (BindIt == Binds.end())
	//	{
	//		Delegate* newdelegate = new Delegate();
	//		newdelegate->Bind(key, wObject, method);
	//		Binds.insert(std::make_pair(ActionName, newdelegate));
	//	}
	//	else
	//	{
	//		assert("Current action already added");
	//	}
	//}

	template< class UserClass, class Func>
	void BindAction(const std::string& ActionName, const FKey Key, const EInputEvent KeyEvent, UserClass* Object, Func Method)
	{
		auto newdelegate = new DelegateNoArgs();
		newdelegate->Bind(Key, KeyEvent, Object, Method);
		auto BindIt = std::find(BindsKeyAction.begin(), BindsKeyAction.end(), newdelegate);

		if (BindIt == BindsKeyAction.end())
		{
			BindsKeyAction.push_back(newdelegate);
		}
		else
		{
			delete newdelegate;
			newdelegate = nullptr;
			assert("Current action already added");
		}
	}

	template< class UserClass, class Func>
	void BindAction(const std::string& ActionName, const EInputEvent KeyEvent, UserClass* Object, Func Method)
	{
		auto newdelegate = new DelegateNoArgs();
		newdelegate->Bind(ActionName, KeyEvent, Object, Method);
		auto BindIt = std::find(BindsKeyAction.begin(), BindsKeyAction.end(), newdelegate);

		if (BindIt == BindsKeyAction.end())
		{
			BindsKeyAction.push_back(newdelegate);
		}
		else
		{
			delete newdelegate;
			newdelegate = nullptr;
			assert("Current action already added");
		}
	}

	template< class UserClass, class Func>
	void BindAxis(const std::string& AxisName, const unsigned char Key, const EInputEvent KeyEvent, UserClass* Object, Func Method)
	{
		auto newdelegate = new DelegateWithOneArg();
		newdelegate->Bind(Key, KeyEvent, 0, Object, Method);
		auto BindIt = std::find(BindsAxisAction.begin(), BindsAxisAction.end(), newdelegate);

		if (BindIt == BindsAxisAction.end())
		{
			BindsAxisAction.push_back(newdelegate);
		}
		else
		{
			delete newdelegate;
			newdelegate = nullptr;
			assert("Current action already added");
		}
	}

	template< class UserClass, class Func>
	void BindAxis(const std::string& AxisName, UserClass* Object, Func Method)
	{
		auto newdelegate = new DelegateWithOneArg();
		newdelegate->Bind(AxisName, Object, Method);
		auto BindIt = std::find(BindsAxisAction.begin(), BindsAxisAction.end(), newdelegate);

		if (BindIt == BindsAxisAction.end())
		{
			BindsAxisAction.push_back(newdelegate);
		}
		else
		{
			delete newdelegate;
			newdelegate = nullptr;
			assert("Current action already added");
		}
	}

	void ExecuteKey(FKey Key,const EInputEvent KeyEvent)
	{
		//auto inputKey = KeyBinding(Key, KeyEvent);

		for (auto & Bind : BindsKeyAction)
		{
			if(Bind->GetKeyInfo().GetKey() == Key && Bind->GetKeyInfo().KeyEvent == KeyEvent)
			{
				Bind->Execute();
			}
		}

	}

	void ExecuteKey(const std::string ActionName, const EInputEvent KeyEvent)
	{
		auto BindIt = std::find_if(BindsKeyAction.begin(), BindsKeyAction.end(),
			[ActionName,KeyEvent](DelegateNoArgs* keyDelegate)
			{
				return (keyDelegate->GetActionName() == ActionName) && (keyDelegate->GetKeyEvent() == KeyEvent);
			});

		if (BindIt != BindsKeyAction.end())
		{
			if (*BindIt != nullptr)
			{
				(*BindIt)->Execute();
			}
		}
	}

	void ExecuteAxis(const std::string AxisName, float Scale)
	{
		auto BindIt = std::find_if(BindsAxisAction.begin(), BindsAxisAction.end(),
			[AxisName](DelegateWithOneArg* axisDelegate)
			{
				return axisDelegate->GetAxisName() == AxisName;
			});

		if(BindIt != BindsAxisAction.end())
		{
			if(*BindIt != nullptr)
			{
				(*BindIt)->Execute(Scale);
			}
		}
	}

public:

	bool IsBlockInput() const;
	void ToggleBlockInput();
	void SetBlockInput(bool bNewBlockInput);

private:
	bool bBlockInput = true;

	std::vector<DelegateNoArgs*> BindsKeyAction;
	std::vector<DelegateWithOneArg*> BindsAxisAction;
};

DECLARE_LOG_CATEGORY_EXTERN(InputLog)