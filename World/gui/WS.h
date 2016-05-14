#pragma once
#include <GL\glew.h>
#include "WindowBase.h"
#include <tools\StringIntern.h>

#include "gui\WindowPerfomance.h"
#include "gui\WindowDb.h"
#include "gui\WindowInventory.h"
#include "gui\WindowRecipe.h"
#include "gui\WindowEventlog.h"
#include "gui\WindowTools.h"
#include "gui\WindowCraftQueue.h"

class World;

class WS
{
public:

	void Draw(glm::vec2 wsize);

	static WS &Get()
	{
		static WS w;
		return w;
	}
};