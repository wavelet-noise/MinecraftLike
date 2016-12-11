#include "WindowOverlay.h"
#include "Window/Window.h"
#include "gui/WindowTools.h"
#include "gui/WindowDb.h"
#include "WindowProfesions.h"
#include "gui/WindowStorages.h"
#include "WindowFinance.h"
#include <Game.h>
#include <Core/MapGen/WorldWorker.h>

void Currency::AddCurrency(int count)
{
	currency += count;
}

int Currency::GetCurrency() const
{
	return currency;
}

int Currency::GetSaldo()
{
	int all = 0;
	for (const auto &c : Game::GetWorker()->w->creatures)
	{
		all -= static_cast<int>(c.second->GetAgent<ProfessionPerformer>()->GetSalary());
	}

	all += 2000;

	return all;
}

void Currency::Update(float dt)
{
	auto nday = EventBus::Get().ListenChannel<EventNewDay>();
	if (!nday.empty())
	{
		currency += GetSaldo();
	}
}

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
	if (ImGui::Button("Professions"))
		WindowProfessions::Get().Toggle();

	ImGui::SameLine();
	if (ImGui::Button("Storages"))
		WindowStorages::Get().Toggle();

	ImGui::SameLine(ImGui::GetWindowWidth() - 200);
	ImGui::Text(IngameDateToString(Game::GetWorker()->w->GetTime()).c_str());

	if (ImGui::Button("Database"))
		WindowDb::Get().Toggle();

	ImGui::SameLine(ImGui::GetWindowWidth() - 100);
	if (ImGui::Button((boost::format("%1% cur##Currency") % Currency::Get().GetCurrency()).str().c_str()))
		WindowFinance::Get().Toggle();

	ImGui::End();
}
