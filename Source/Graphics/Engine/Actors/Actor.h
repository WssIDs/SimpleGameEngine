#pragma once
#include "..\Core.h"
#include <vector>
#include <memory>
#include "..\..\Test\InputSystem.h"


class Actor : public WObject
{
public:

	void CreatePlayerInputComponent();

	virtual void SetupPlayerInputComponent(InputSystem* InputComponent);

	virtual void Tick(double deltaTime) override;
	void Render(double deltaTime) override;

	//template<typename T>
	//std::shared_ptr<T> NewObject(const std::string& name)
	//{
	//	std::shared_ptr<T> newObject = std::make_shared<T>();
	//	newObject.SetName(name);

	//	ChildObjects.push_back(newObject);
	//	return newObject;
	//}
	//} 

	void DisableInput();
	void EnableInput();
	void ToggleInput();
	bool IsBlockInput() const;

protected:

	/// position
	/// rotation
	/// scale
	/// transform
	/// 
	/// 

private:

	std::shared_ptr<InputSystem> InputComponent;
};

