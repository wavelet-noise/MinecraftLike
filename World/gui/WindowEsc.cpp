#include "WindowEsc.h"
#include <Game.h>
#include <Core/MapGen/WorldWorker.h>

WindowEsc::WindowEsc()
{
}

void WindowEsc::Draw(glm::vec2 mainwin_size, float gt)
{
	if (mOpen)
	{
		ImGui::SetNextWindowPosCenter();
		ImGui::SetNextWindowSize({200,400});
		ImGui::Begin("esc_menu", &mOpen, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar);
		if (ImGui::Button("Save"))
			Game::GetWorker()->SaveAll();
		ImGui::End();
	}
}
