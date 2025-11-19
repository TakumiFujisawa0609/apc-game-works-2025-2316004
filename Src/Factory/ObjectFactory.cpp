#include "../pch.h"
#include "../Object/ObjectBase.h"
#include "ObjectFactory.h"

std::unique_ptr<ObjectBase> ObjectFactory::CreateObject(OBJECT_TYPE type)
{

	auto it = factoryMap_.find(type);
	if (it != factoryMap_.end())
	{
		return it->second();
	}
	return nullptr;

}
