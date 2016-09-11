#pragma once
#include "WindowBase.h"
#include <tools\StringIntern.h>
#include <Core/Recipe.h>

class World;

class WindowRecipe : public WindowBase
{
public:

	WindowRecipe();

	// Унаследовано через WindowBase
	void Draw(glm::vec2 mainwin_size, float gt) override;

	void ShowRecipe(const StringIntern &s);
	void ShowUsing(const StringIntern &s);
	void ShowDeepRecipe(const StringIntern& s);

	enum Cur {
		RECIPE,
		USAGE,
		DEEP
	} current = RECIPE;
	StringIntern what;
	bool ready = false;
	PDeepRecipe dr;

	static WindowRecipe &Get()
	{
		static WindowRecipe w;
		return w;
	}
};