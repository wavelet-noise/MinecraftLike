#pragma once
#include "WindowBase.h"
#include <tools\StringIntern.h>

class Room;
class World;

class WindowRooms : public WindowBase
{
public:

	WindowRooms();

	World * w;

	// Унаследовано через WindowBase
	virtual void Draw(glm::vec2 mainwin_size) override;

	static WindowRooms &Get()
	{
		static WindowRooms wr;
		return wr;
	}

	std::shared_ptr<Room> selected;
};