#pragma once
#include <GL\glew.h>
#include "WindowBase.h"
#include <tools\StringIntern.h>
#include <boost\circular_buffer.hpp>

class World;

class WindowEventlog : public WindowBase
{
public:

	WindowEventlog();

	// Унаследовано через WindowBase
	virtual void Draw(glm::vec2 mainwin_size, float gt) override;

	static WindowEventlog &Get()
	{
		static WindowEventlog w;
		return w;
	}

private:
	boost::circular_buffer<std::string> evlog;
};