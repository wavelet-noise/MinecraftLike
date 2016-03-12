#include "WindowInventory.h"
#include <Core\World.h>
#include <Core\Chest.h>
#include <Render\TextureManager.h>
#include <DragNDrop.h>

WindowInventory::WindowInventory()
{
}

void WindowInventory::Draw()
{
  ImGui::SetNextWindowSize(ImVec2(500, 400), ImGuiSetCond_FirstUseEver);
  ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiSetCond_FirstUseEver);

  ImGui::Begin("Inventory", &mOpen);
  w->GetPlayer()->GetFromFullName<Chest>("Chest")->DrawGui();
  ImGui::End();
}
