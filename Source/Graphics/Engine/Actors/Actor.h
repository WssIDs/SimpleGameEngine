#pragma once
#include "..\Core.h"
#include <vector>
#include <memory>


class Actor : public WObject
{
public:

	virtual void Tick(double deltaTime) override;
	void Render(double deltaTime) final;

	//template<typename T>
	//std::shared_ptr<T> NewObject(const std::string& name)
	//{
	//	std::shared_ptr<T> newObject = std::make_shared<T>();
	//	newObject.SetName(name);

	//	ChildObjects.push_back(newObject);
	//	return newObject;
	//}

protected:

	/// position
	/// rotation
	/// scale
	/// transform
};

