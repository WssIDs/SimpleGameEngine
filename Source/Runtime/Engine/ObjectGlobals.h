#pragma once
#include <memory>
//#include "WObject.h"



//template<typename T>
//std::shared_ptr<T> NewObject(WObject* Outer, const std::string& name)
//{
//	std::shared_ptr<T> newObject = std::make_shared<T>();
//	
//	newObject->SetOuter(Outer);
//	newObject->SetName(name);
//	Outer->GetChildObjects().push_back(newObject);
//
//	return newObject;
//}