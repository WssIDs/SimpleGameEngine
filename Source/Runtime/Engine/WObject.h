#pragma once
#include "IWObject.h"

class Graphics;


class WObject : public IWObject
{
public:

	WObject();

	~WObject();
	
	virtual void Tick(double deltaTime) override;
	virtual void Render(double deltaTime) override;

	std::string GetName() const;
	WObject* GetOuter() const;
	std::vector<std::shared_ptr<WObject>> GetChildObjects();

	void SetName(const std::string& name);
	void SetOuter(WObject* inOuter);

	template<typename T>
	std::shared_ptr<T> NewObject(WObject* Outer, const std::string& name)
	{
		std::shared_ptr<T> newObject = std::make_shared<T>();
		newObject->SetOuter(Outer);
		newObject->SetName(name);
		ChildObjects.push_back(newObject);
		return newObject;
	}
protected:

private:
	std::vector<std::shared_ptr<WObject>> ChildObjects;
	WObject* Outer;
	std::string Name;
};