#pragma once
#include <tools\StringIntern.h>
#include "gui/WindowBase.h"
#include "gui/WS.h"
#include "Core/World.h"
#include "Core/PositionAgent.h"

struct Room;
class World;

class WindowProfessions : public WindowBase
{
public:

	WindowProfessions();

	// Унаследовано через WindowBase
	void Draw(glm::vec2 mainwin_size, float gt) override;
	int selected_column = 0;

	static WindowProfessions &Get()
	{
		static WindowProfessions wr;
		return wr;
	}
};
