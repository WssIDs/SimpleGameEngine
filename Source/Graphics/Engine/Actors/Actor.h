#pragma once
#include "..\Core\WObject.h"
#include <vector>
#include <memory>


class Actor : public WObject
{
public:

	virtual void Tick(double deltaTime) override;
	void Render(double deltaTime) final;

	std::vector<std::shared_ptr<WObject>> GetChildObjects();

protected:

	std::vector<std::shared_ptr<WObject>> ChildObjects;

	/// position
	/// rotation
	/// scale
	/// transform
};

