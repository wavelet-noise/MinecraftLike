#include "WindowDb.h"
#include "imconfig.h"
#include <glm\glm.hpp>
#include <Core\DB.h>
#include <Render\TextureManager.h>
#include "WindowInventory.h"
#include <Core\World.h>
#include <Core\agents\Chest.h>
#include <GLFW\glfw3.h>
#include <gui\WindowRecipe.h>
#include "WindowTools.h"
#include <DragNDrop.h>

void WindowDb::Draw(glm::vec2 mainwin_size, float gt)
{
	int jj = 666;
	if (mOpen)
	{
		ImGui::SetNextWindowSize(ImVec2(500, mainwin_size.y - 20), ImGuiSetCond_Always);
		ImGui::SetNextWindowPos(ImVec2(mainwin_size.x - 500, 0), ImGuiSetCond_Always);

		ImGui::Begin("Database", &mOpen, ImGuiWindowFlags_AlwaysAutoResize);
		ImGui::InputText("Filter", flt, 100);
		ImGui::Checkbox("Only craftable", &only_craftable);

		ImDrawList* draw_list = ImGui::GetWindowDrawList();

		auto p = ImGui::GetWindowPos();
		auto craftable_it = DB::Get().mRecipeCache.begin();
		auto all_it = DB::Get().mObjects.begin();

		bool cont = true;
		while (cont)
		{
			StringIntern id;
			std::tuple<PGameObject, bool> pgo;
			if (only_craftable)
			{
				id = craftable_it->first;
				auto f = DB::Get().mObjects.find(id);

				craftable_it++;
				if (craftable_it == DB::Get().mRecipeCache.end())
					cont = false;
				if (f == DB::Get().mObjects.end())
					continue;

				pgo = f->second;
			}
			else
			{
				id = all_it->first;
				pgo = all_it->second;
				all_it++;
				if (all_it == DB::Get().mObjects.end())
					cont = false;
			}

			if (!std::get<0>(pgo))
				continue;

			if (flt)
			{
				auto fstring = std::get<0>(pgo)->GetDescription();
				if (fstring.find(std::string(flt)) == -1)
					continue;
			}

			auto &atl = TextureManager::Get().GetTexture(id);
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
				ImGui::SetTooltip("%s\n%s", id.get().c_str(), std::get<0>(pgo)->GetDescription().c_str());

				if (ImGui::IsKeyPressed(GLFW_KEY_R))
				{
					WindowRecipe::Get().ShowRecipe(id);
				}

				if (ImGui::IsKeyPressed(GLFW_KEY_U))
				{
					WindowRecipe::Get().ShowUsing(id);
				}

				if (ImGui::IsKeyPressed(GLFW_KEY_R) && ImGui::IsKeyDown(GLFW_KEY_LEFT_SHIFT))
				{
					WindowRecipe::Get().ShowDeepRecipe(id);
				}

				if (ImGui::IsMouseClicked(0))
				{
					selected_id = std::get<0>(pgo)->GetId();
				}

				auto &dnd = DragNDrop::Get();
				if (ImGui::IsItemHovered())
				{
					if (!dnd.Busy())
						dnd.Drag({ DB::Get().Create(id), 999});
				}
			}
		}
		ImGui::End();
	}
}
