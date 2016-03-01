#include "WindowInventory.h"
#include <Core\World.h>
#include <Core\Chest.h>
#include <Render\TextureManager.h>

WindowInventory::WindowInventory()
{
}

void WindowInventory::Draw()
{
  ImGui::SetNextWindowSize(ImVec2(500, 400), ImGuiSetCond_FirstUseEver);
  ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiSetCond_FirstUseEver);

  ImGui::Begin("Inventory", &mOpen);

  int jj = 666;
  auto p = ImGui::GetWindowPos();
  for (const auto &a : w->GetPlayer()->GetFromFullName<Chest>("Chest")->slots)
  {
    if (jj < 9)
      ImGui::SameLine(), jj++;
    else
      jj = 0;

    if (!a.obj)
    {
      ImGui::Button("", { 40, 38 });
    }
    else if (auto i = DB::Get().GetModel(a.obj->GetId()))
    {
      Model &m = *i;
      auto &atl = TextureManager::Get().GetTexture(m.GetSpriteName());
      auto &tex = std::get<0>(atl);

      //some shit, remake stored uv to texture coord space
      auto uv = glm::vec2(std::get<1>(atl).x, std::get<1>(atl).y) / glm::vec2(tex->GetSize());
      auto uv2 = uv + glm::vec2(std::get<1>(atl).z, std::get<1>(atl).w) / glm::vec2(tex->GetSize());
      std::swap(uv.x, uv2.x);


      ImGui::ImageButton(reinterpret_cast<ImTextureID>(tex->GetId()), { 32,32 }, uv2, uv);
      if (ImGui::IsItemHovered())
      {
        ImGui::SetTooltip("%s x%d", a.obj->GetId().get().c_str(), a.count);
      }
    }
  }
  ImGui::End();
}
