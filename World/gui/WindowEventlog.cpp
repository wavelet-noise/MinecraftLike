#include "WindowEventlog.h"
#include <Core\EventBus.h>
#include <Game.h>

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

		ImGui::SetNextWindowPos({ 0,0 });
		ImGui::SetNextWindowSize({Game::GetWindow()->GetSize().x / 2.f ,Game::GetWindow()->GetSize().y / 1.f});
		ImGui::Begin("Eventlog", &mOpen, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar);
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
