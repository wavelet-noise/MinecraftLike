#include "WindowEventlog.h"
#include <Core\EventBus.h>

WindowEventlog::WindowEventlog()
{
	mOpen = false;
	evlog.resize(100);
}

void WindowEventlog::Draw(glm::vec2 mainwin_size, float gt)
{
	if (mOpen)
	{
		for (const auto &e : EventBus::Get().ListenChannel<Event>())
		{
			evlog.push_back(e);
		}

		ImGui::SetNextWindowSize({500,400});
		ImGui::Begin("Eventlog", &mOpen);
		for (const auto &s : evlog)
		{
			if(s)
			{
				ImGui::Text(s->to_string().c_str());
			}
		}
		ImGui::End();
	}
}
