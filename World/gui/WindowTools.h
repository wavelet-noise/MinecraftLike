#pragma once
#include "WindowBase.h"
#include <tools\StringIntern.h>

class World;

enum class SelectedOrder
{
	NONE,
	DIG_SQUARE,
	COLLECT_SQUARE,
	PLACE_BLOCK
};

class WindowTools : public WindowBase
{
public:

	WindowTools();

	// Унаследовано через WindowBase
	virtual void Draw(glm::vec2 mainwin_size) override;

	static WindowTools &Get()
	{
		static WindowTools w;
		return w;
	}

	SelectedOrder selected;
};