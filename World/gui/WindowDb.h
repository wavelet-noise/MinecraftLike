#pragma once
#include "WindowBase.h"
#include <tools\StringIntern.h>

class World;

class WindowDb : public WindowBase
{
public:

	World * w;

	// Унаследовано через WindowBase
	void Draw(glm::vec2 mainwin_size, float gt) override;

	StringIntern GetSelectedId() const
	{
		return selected_id;
	}

	static WindowDb &Get()
	{
		static WindowDb w;
		return w;
	}

private:
	StringIntern selected_id;
	char flt[100] = { 0 };
};