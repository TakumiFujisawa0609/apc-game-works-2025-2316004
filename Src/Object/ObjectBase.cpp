#include "../Manager/Resource/ResourceManager.h"
#include "../Manager/Generic/SceneManager.h"
#include "ObjectBase.h"

ObjectBase::ObjectBase(void)
	: resMng_(ResourceManager::GetInstance()),
	scnMng_(SceneManager::GetInstance())
{
}

ObjectBase::~ObjectBase(void)
{
}
