#pragma once
#include "WindowBase.h"
#include <list>
#include <Core/GameObject.h>

class World;

class Storages
{
public:
	std::list<std::tuple<glm::vec3, PGameObject>>& List();
	void AddStorage(glm::vec3 pos, PGameObject go);
	void RemoveStorage(PGameObject go);

	static Storages &Get()
	{
		static Storages w;
		return w;
	}

private:
	std::list<std::tuple<glm::vec3, PGameObject>> storages;
};

class WindowStorages : public WindowBase
{
public:

	WindowStorages();

	// Унаследовано через WindowBase
	virtual void Draw(glm::vec2 mainwin_size, float gt) override;

	static WindowStorages &Get()
	{
		static WindowStorages w;
		return w;
	}
};