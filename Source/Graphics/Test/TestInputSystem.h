#pragma once
#include <vector>
#include <string>
#include <cassert>
#include <map>
#include "..\Engine\Core.h"
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
		Key(0),
		KeyEvent(EInputEvent::IE_MAX)
	{}

	KeyBinding(unsigned char Key, EInputEvent KeyEvent)
		:
		Key(Key),
		KeyEvent(KeyEvent)
	{
	}

	unsigned char Key;
	EInputEvent KeyEvent;
};

struct AxisBinding
{
	AxisBinding()
		:
		Scale(0.0f),
		MaxScale(0.0f),
		Key(0),
		KeyEvent(EInputEvent::IE_MAX)
	{}

	AxisBinding(unsigned char Key, EInputEvent KeyEvent)
		:
		Scale(0.0f),
		MaxScale(0.0f),
		Key(Key),
		KeyEvent(KeyEvent)
	{}

	AxisBinding(unsigned char Key, EInputEvent KeyEvent, float MaxScale)
		:
		Scale(0.0f),
		MaxScale(MaxScale),
		Key(Key),
		KeyEvent(KeyEvent)
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

	float MaxScale = 0.0f;
	float Scale = 0.0f;
	unsigned char Key;
	EInputEvent KeyEvent;
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
	void Bind(const unsigned char Key, const EInputEvent KeyEvent, UserClass* Object, Func Method)
	{
		if(DelegateContainer)
		{
			delete DelegateContainer;
		}
		
		Binding = KeyBinding(Key, KeyEvent);
		DelegateContainer = new Container<UserClass, Func>(Object, Method);
	}

	void Execute()
	{
		DelegateContainer->Execute(&Arguments<>());
	}

	//template< class T1 > void operator()(T1 i_arg1)
	//{
	//	m_container->Call(&Arguments< T1 >(i_arg1));
	//}

	//template< class T1, class T2 > void operator()(T1 i_arg1, T2 i_arg2)
	//{
	//	m_container->Call(&Arguments< T1, T2 >(i_arg1, i_arg2));
	//}

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
	void Bind(const unsigned char Key, const EInputEvent KeyEvent, float MaxScale, UserClass* Object, Func Method)
	{
		if (DelegateContainer)
		{
			delete DelegateContainer;
		}

		Binding = AxisBinding(Key, KeyEvent, MaxScale);
		DelegateContainer = new Container<UserClass, Func>(Object, Method);
	}

	template<class Arg>
	void Execute(Arg Args)
	{
		DelegateContainer->Execute(&Arguments<Arg>(Args));
	}

	//template< class T1 > void operator()(T1 i_arg1)
	//{
	//	m_container->Call(&Arguments< T1 >(i_arg1));
	//}

	//template< class T1, class T2 > void operator()(T1 i_arg1, T2 i_arg2)
	//{
	//	m_container->Call(&Arguments< T1, T2 >(i_arg1, i_arg2));
	//}

	AxisBinding GetAxisInfo() const
	{
		return Binding;
	}

private:
	AxisBinding Binding;
	IContainer* DelegateContainer;
};

class TestInputSystem
{
public:
	TestInputSystem(const TestInputSystem&) = delete;
	TestInputSystem& operator=(const TestInputSystem&) = delete;

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
	void BindAction(const std::string& ActionName, const unsigned char Key, const EInputEvent KeyEvent, UserClass* Object, Func Method)
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
	void BindAxis(const std::string& ActionName, const unsigned char Key, const EInputEvent KeyEvent, UserClass* Object, Func Method)
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

	void ExecuteKey(const unsigned char Key,const EInputEvent KeyEvent)
	{
		//auto inputKey = KeyBinding(Key, KeyEvent);

		for (auto & Bind : BindsKeyAction)
		{
			if(Bind->GetKeyInfo().Key == Key && Bind->GetKeyInfo().KeyEvent == KeyEvent)
			{
				Bind->Execute();
			}
		}

	}

	void ExecuteAxis(const unsigned char Key, const EInputEvent KeyEvent)
	{
		for (auto& Bind : BindsAxisAction)
		{
			if (Bind->GetAxisInfo().Key == Key && Bind->GetAxisInfo().KeyEvent == KeyEvent)
			{
				Bind->Execute(Bind->GetAxisInfo().Scale);
			}
		}
	}

	void ExecuteAxis(const unsigned char Key, const EInputEvent KeyEvent, float Scale)
	{
		for (auto& Bind : BindsAxisAction)
		{
			if (Bind->GetAxisInfo().Key == Key && Bind->GetAxisInfo().KeyEvent == KeyEvent)
			{
				Bind->Execute(Scale);
			}
		}
	}

protected:
	TestInputSystem() {};
public:
	static TestInputSystem& Get();

private:

	std::vector<DelegateNoArgs*> BindsKeyAction;
	std::vector<DelegateWithOneArg*> BindsAxisAction;
};

DECLARE_LOG_CATEGORY_EXTERN(InputLog)