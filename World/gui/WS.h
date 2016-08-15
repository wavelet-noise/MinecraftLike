#pragma once
#include <GL/glew.h>
#include "WindowBase.h"

class World;

class WS
{
public:

	void Draw(glm::vec2 wsize, float gt);

	World * w;

	static WS &Get()
	{
		static WS w;
		return w;
	}
};