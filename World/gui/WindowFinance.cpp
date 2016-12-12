#include "WindowFinance.h"
#include <Core\World.h>
#include "WS.h"
#include "WindowOverlay.h"
#include <Core/PositionAgent.h>
#include <Game.h>
#include <Core/MapGen/WorldWorker.h>

WindowFinance::WindowFinance()
{
}

void WindowFinance::Draw(glm::vec2 mainwin_size, float gt)
{
	if (mOpen)
	{
		ImGui::SetNextWindowSize(ImVec2(500, 400));
		ImGui::SetNextWindowPosCenter();

		ImGui::Begin("WindowFinance", &mOpen, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);
		
		ImGui::TextColored(Color::LightGreen, "%d cur", Currency::Get().GetCurrency());
		ImGui::Separator();

		int crea_all = 0;
		for (const auto &c : Game::GetWorker()->w->creatures)
		{
			crea_all += static_cast<int>(c.second->GetAgent<ProfessionPerformer>()->GetSalary());
		}

		if (ImGui::TreeNode("Creatures", "Creatures (-%d)", crea_all))
		{
			for (const auto &c : Game::GetWorker()->w->creatures)
			{
				int sal = static_cast<int>(c.second->GetAgent<ProfessionPerformer>()->GetSalary());
				if (sal > 0)
				{
					ImGui::Text(c.second->GetAgent<Named>()->name.c_str());
					ImGui::SameLine();
					ImGui::TextColored(Color::LightGreen, "-%d cur", sal);
				}
			}
			ImGui::TreePop();
		}
		
		int found_all = 2000;
		if(ImGui::TreeNode("Financing", "Financing (%d)", found_all))
		{
			ImGui::Text("Constant financing");
			ImGui::SameLine();
			ImGui::TextColored(Color::LightGreen, "%d cur", found_all);

			ImGui::TreePop();
		}

		ImGui::Separator();
		ImGui::Text("Total");
		ImGui::SameLine();
		ImGui::TextColored(Color::LightGreen, "%d cur", Currency::Get().GetSaldo());

		ImGui::End();
	}
}
