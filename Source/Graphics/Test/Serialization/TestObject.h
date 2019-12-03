#pragma once
#include <vector>
#include <string>
#include <fstream>

#include <boost/serialization/vector.hpp>


class TestObject
{
public:
	TestObject();

	void PrintAll();
	void Change();

private:
	friend class boost::serialization::access;

	template<class Archive>
	void serialize(Archive& ar, const unsigned int version)
	{
		ar& width;
		ar& height;
		ar& delta;
		ar& Name;
		ar& numbers;
	}


	int width;
	int height;
	float delta;
	
	std::string Name;
	std::vector<int> numbers;

};

