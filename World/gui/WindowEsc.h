#pragma once
#include "WindowBase.h"
#include <tools\StringIntern.h>
#include "WindowRooms.h"

struct Room;
class World;

class WindowEsc : public WindowBase
{
public:

	WindowEsc();

	// Унаследовано через WindowBase
	virtual void Draw(glm::vec2 mainwin_size, float gt) override;

	static WindowEsc &Get()
	{
		static WindowEsc wr;
		return wr;
	}
};
