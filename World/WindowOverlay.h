#pragma once
#include <gui\WindowBase.h>
#include <tools\StringIntern.h>

struct Room;
class World;

class WindowOverlay : public WindowBase
{
public:

	WindowOverlay();

	// Унаследовано через WindowBase
	void Draw(glm::vec2 mainwin_size, float gt) override;

	static WindowOverlay &Get()
	{
		static WindowOverlay wr;
		return wr;
	}
};