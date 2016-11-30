#include "WindowProfesions.h"
#include "VerticalText.h"
#include "Gatherer.h"
#include "Cooker.h"
#include "Medic.h"
#include "Miner.h"

WindowProfessions::WindowProfessions()
{
}

void WindowProfessions::Draw(glm::vec2 mainwin_size, float gt)
{
	static const std::vector<PProfession> columns = {
		std::make_shared<Gatherer>(),
		std::make_shared<Cooker>(),
		std::make_shared<Medic>(),
		std::make_shared<Miner>(),
	};

	if (mOpen)
	{
		ImGui::SetNextWindowSize({ 200.f + columns.size() * 25, 400.f });
		ImGui::Begin("Professions", &mOpen);

		ImGui::Columns(2 + columns.size(), "mycolumns");
		ImGui::SetColumnOffset(-1, 0);
		ImGui::Text(" ");
		ImGui::NextColumn();

		for (int i = 0; i < columns.size(); i++)
		{
			ImGui::SetColumnOffset(-1, 200 + i * 25);
			ImGui::TextVertical(columns[i]->Name().c_str());
			{
				//reverse = (comparison == pathCompare) ? !reverse : false;
				//comparison == pathCompare;
			}
			ImGui::NextColumn();
		}
		ImGui::SetColumnOffset(-1, 200 + columns.size() * 25);
		ImGui::Text("");
		ImGui::NextColumn();

		ImGui::Separator();

		auto &w = WS::Get().w;
		int i = 0, j = 0;
		if (!w->creatures.empty())
			for (const auto &c : w->creatures)
			{
				ImGui::Text(c.second->GetAgent<Named>()->name.c_str());
				ImGui::NextColumn();

				auto &vec = c.second->GetAgent<ProfessionPerformer>()->prof;
				std::vector<PProfession> new_pp;

				for (const auto &col : columns)
				{
					auto finded = std::find_if(vec.begin(), vec.end(), [&](const PProfession &pp)->bool {return pp->IsEquals(*col); });
					if (finded != vec.end())
					{
						if (ImGui::Selectable((boost::format("%3%##%1%_%2%") % i % j % static_cast<int>((*finded)->GetLevel())).str().c_str(), (*finded)->GetActive()))
						{
							(*finded)->SetActive(!(*finded)->GetActive());
						}
					}
					else
					{
						ImGui::Selectable((boost::format("E##%1%_%2%") % i % j).str().c_str(), false);
					}

					ImGui::NextColumn();
					j++;
				}
				i++;
				ImGui::NextColumn();
			}

		ImGui::End();
	}
}