#pragma once
#include "WindowBase.h"
#include <tools\StringIntern.h>

class World;

class WindowEventlog : public WindowBase
{
public:

	WindowEventlog();

	// Унаследовано через WindowBase
	virtual void Draw(glm::vec2 mainwin_size) override;

	static WindowEventlog &Get()
	{
		static WindowEventlog w;
		return w;
	}
};