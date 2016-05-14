#pragma once
#include "WindowBase.h"
#include <tools\StringIntern.h>

class World;

class WindowDb : public WindowBase
{
public:

	World * w;

	// Унаследовано через WindowBase
	virtual void Draw(glm::vec2 mainwin_size) override;

	StringIntern GetSelectedId()
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
};