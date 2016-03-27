#pragma once
#include "WindowBase.h"
#include <tools\StringIntern.h>

class World;

class WindowRecipe : public WindowBase
{
public:

	WindowRecipe();

	// Унаследовано через WindowBase
	virtual void Draw(glm::vec2 mainwin_size) override;

	void ShowRecipe(const StringIntern &s);
	void ShowUsing(const StringIntern &s);

	bool recipe = true;
	StringIntern what;

	static WindowRecipe &Get()
	{
		static WindowRecipe w;
		return w;
	}
};