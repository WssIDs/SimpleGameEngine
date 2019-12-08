#pragma once
#include <vector>
#include <string>
#include <fstream>
#include "boost\serialization\binary_object.hpp"
#include <boost/serialization/base_object.hpp>
#include <boost/type_traits/is_abstract.hpp>


class Graphics;
class Model;

class IWObject
{
public:
	virtual ~IWObject() {};
	virtual void Tick(Graphics& gfx, double deltaTime) = 0;
	virtual void Render(Graphics& gfx, double deltaTime) = 0;
	
private:
	friend class boost::serialization::access;
	
	template<class Archive>
	void serialize(Archive& ar, const unsigned int version)
	{
		//ar.template register_type<Model>();
	}
};
