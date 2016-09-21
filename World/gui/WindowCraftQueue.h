#pragma once
#include "WindowBase.h"

class World;

class WindowCraftQueue : public WindowBase
{
public:

	WindowCraftQueue();

	// Унаследовано через WindowBase
	virtual void Draw(glm::vec2 mainwin_size, float gt) override;

	static WindowCraftQueue &Get()
	{
		static WindowCraftQueue w;
		return w;
	}
};