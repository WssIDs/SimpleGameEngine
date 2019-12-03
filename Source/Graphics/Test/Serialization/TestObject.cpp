#include "TestObject.h"
#include <iostream>

TestObject::TestObject()
{
	width = 100;
	height = 200;
	delta = 0.001f;

	Name = "TestObject";
	
	for (int i = 0; i < 10; i++)
	{
		numbers.push_back(i);
	}
}

void TestObject::PrintAll()
{
	std::cout << "width = " << width << std::endl;
	std::cout << "height = " << height << std::endl;
	std::cout << "delta = " << delta << std::endl;
	std::cout << "Name = " << Name << std::endl;

	std::cout << "numbers" << std::endl;
	for (int i = 0; i < 10; i++)
	{
		std::cout << "number = " << numbers[i] << std::endl;
	}
}

void TestObject::Change()
{
	width = 150;
	height = 260;
	delta = 1.011f;

	Name = "TestObjectClass";

	for (int i = 0; i < 10; i++)
	{
		numbers.push_back(i*i);
	}
}
