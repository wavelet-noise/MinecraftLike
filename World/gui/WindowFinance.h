#pragma once
#include "WindowBase.h"

class World;

class WindowFinance : public WindowBase
{
public:

	WindowFinance();

	// Унаследовано через WindowBase
	virtual void Draw(glm::vec2 mainwin_size, float gt) override;

	static WindowFinance &Get()
	{
		static WindowFinance w;
		return w;
	}
};