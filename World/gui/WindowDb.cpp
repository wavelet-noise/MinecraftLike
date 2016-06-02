#include "WindowDb.h"
#include "imconfig.h"
#include <glm\glm.hpp>
#include <Core\DB.h>
#include <Render\TextureManager.h>
#include "WindowInventory.h"
#include <Core\World.h>
#include <Core\Chest.h>
#include <GLFW\glfw3.h>
#include <gui\WindowRecipe.h>
#include "WindowTools.h"

void WindowDb::Draw(glm::vec2 mainwin_size)
{
	ImGui::SetNextWindowSize(ImVec2(500, mainwin_size.y - 20), ImGuiSetCond_Always);
	ImGui::SetNextWindowPos(ImVec2(mainwin_size.x - 500, 0), ImGuiSetCond_Always);

	int jj = 666;
	if (mOpen)
	{
		ImGui::Begin("Database", &mOpen, ImGuiWindowFlags_AlwaysAutoResize);
		ImGui::InputText("Filter", flt, 100);

		ImDrawList* draw_list = ImGui::GetWindowDrawList();

		auto p = ImGui::GetWindowPos();
		for (const auto &a : DB::Get().mObjects)
		{
			if (flt)
			{
				auto fstring = std::get<0>(a.second)->GetDescription();
				if (fstring.find(std::string(flt)) == -1)
					continue;
			}

			auto &atl = TextureManager::Get().GetTexture(a.first);
			auto &tex = std::get<0>(atl);
			auto &atluv = std::get<1>(atl);

			auto uv = glm::vec2(atluv.x, atluv.y);
			auto uv2 = glm::vec2(atluv.z, atluv.w);

			if (jj < 9)
				ImGui::SameLine(), jj++;
			else
				jj = 0;

			ImGui::ImageButton(reinterpret_cast<ImTextureID>(tex->GetId()), { 32,32 }, uv2, uv);

			if (ImGui::IsItemHovered())
			{
				ImGui::SetTooltip("%s\n%s", a.first.get().c_str(), std::get<0>(a.second)->GetDescription().c_str());

				if (ImGui::IsKeyPressed(GLFW_KEY_R))
				{
					WindowRecipe::Get().ShowRecipe(a.first);
				}

				if (ImGui::IsKeyPressed(GLFW_KEY_U))
				{
					WindowRecipe::Get().ShowUsing(a.first);
				}

				if (ImGui::IsMouseClicked(0))
				{
					selected_id = std::get<0>(a.second)->GetId();
				}
			}

		}
		ImGui::End();
	}
}
