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
      auto i = DB::Get().mModel.find(a.first);
      if (i != DB::Get().mModel.end())
      {
        Model &m = *i->second;
        auto &atl = TextureManager::Get().GetTexture(m.GetSpriteName());
        auto &tex = std::get<0>(atl);

        //some shit, remake stored uv to texture coord space
        auto uv = glm::vec2(std::get<1>(atl).x, std::get<1>(atl).y) / glm::vec2(tex->GetSize());
        auto uv2 = uv + glm::vec2(std::get<1>(atl).z, std::get<1>(atl).w) / glm::vec2(tex->GetSize());
        std::swap(uv.x, uv2.x);

        if (jj < 9)
          ImGui::SameLine(), jj++;
        else
          jj = 0;

        ImGui::ImageButton(reinterpret_cast<ImTextureID>(tex->GetId()), { 32,32 }, uv2, uv);
        if (ImGui::IsItemHovered())
        {
          ImGui::SetTooltip("%s", a.first.get().c_str());

          if(ImGui::IsMouseClicked(0))
            WindowInventory::Get().w->GetPlayer()->GetFromFullName<Chest>("Chest")->Push( DB::Get().Create(a.first) );
        }
      }
    }
  }
  ImGui::End();
}
