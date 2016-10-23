#include "WindowOverlay.h"
#include "Window/Window.h"
#include "gui/WindowTools.h"
#include "gui/WindowDb.h"
#include "WindowProfesions.h"


WindowOverlay::WindowOverlay()
{
}

void WindowOverlay::Draw(glm::vec2 mainwin_size, float gt)
{
	ImGui::SetNextWindowSize({0.f + Window::GetSize().x, 100.f});
	ImGui::SetNextWindowPos({0.f, Window::GetSize().y - 100.f});
	ImGui::Begin("Overlay", &mOpen, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar);

	if (WindowTools::Get().selected == SelectedOrder::NONE)
		ImGui::Text("LKM: Observation");
	else
		ImGui::Text("LKM: %s", WindowTools::SelectedOrderName(WindowTools::Get().selected).c_str());

	ImGui::SameLine();
	if (ImGui::Button("Database"))
		WindowDb::Get().Toggle();

	ImGui::SameLine();
	if (ImGui::Button("Professions"))
		WindowProfessions::Get().Toggle();


	ImGui::End();
}
