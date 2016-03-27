#include "WindowInventory.h"
#include <Core\World.h>
#include <Core\Chest.h>
#include <Render\TextureManager.h>
#include <DragNDrop.h>

WindowInventory::WindowInventory()
{
}

void WindowInventory::Draw(glm::vec2 mainwin_size)
{
  ImGui::SetNextWindowSize(ImVec2(500, 400), ImGuiSetCond_FirstUseEver);
  ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiSetCond_FirstUseEver);

  ImGui::Begin("Inventory", &mOpen, ImGuiWindowFlags_AlwaysAutoResize);
  w->GetPlayer()->GetFromFullName<Chest>("Chest")->DrawGui();
  ImGui::End();
}
