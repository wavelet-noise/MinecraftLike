#include "WindowDb.h"
#include "imconfig.h"
#include <glm\glm.hpp>
#include <Core\DB.h>
#include <Render\TextureManager.h>
#include "WindowInventory.h"
#include <Core\World.h>
#include <Core\Chest.h>

void WindowDb::Draw()
{
  ImGui::SetNextWindowSize(ImVec2(500, 400), ImGuiSetCond_FirstUseEver);
  ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiSetCond_FirstUseEver);

  int jj = 666;
  ImGui::Begin("Database", &mOpen);
  {
    ImDrawList* draw_list = ImGui::GetWindowDrawList();

    auto p = ImGui::GetWindowPos();
    for (const auto &a : DB::Get().mObjects)
    {
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

        if (ImGui::IsMouseClicked(0))
          WindowInventory::Get().w->GetPlayer()->GetFromFullName<Chest>("Chest")->Push(DB::Get().Create(a.first));
      }

    }
  }
  ImGui::End();
}
