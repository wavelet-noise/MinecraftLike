#include "WindowEventlog.h"
#include <Core\EventBus.h>

WindowEventlog::WindowEventlog()
{
	mOpen = false;
	evlog.resize(1000);
}

void WindowEventlog::Draw(glm::vec2 mainwin_size)
{
	if (mOpen)
	{
		for (const auto &e : EventBus::Get().ListenChannel<Event>())
		{
			evlog.push_back(e->to_string());
		}

		ImGui::SetNextWindowSize({500,400});
		ImGui::Begin("Eventlog", &mOpen);
		for (const auto &s : evlog)
		{
			ImGui::Text(s.c_str());
		}
		ImGui::End();
	}
}
