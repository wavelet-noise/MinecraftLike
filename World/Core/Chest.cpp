#include "Chest.h"
#include <Serealize.h>
#include <imgui.h>
#include <Core\DB.h>
#include <Render\TextureManager.h>
#include <DragNDrop.h>

Chest::Chest()
  : Agent(nullptr, "Chest", "")
{
  slots.resize(size);
}

void Chest::JsonLoad(const rapidjson::Value &val)
{
  JSONLOAD(NVP(columns), NVP(size));
  slots.resize(size);
}

PAgent Chest::Clone(GameObject * parent, const std::string &name)
{
  auto t = MakeAgent<Chest>(*this);
  t->mParent = parent;
  return t;
}

void Chest::Update(const GameObjectParams & params)
{
}

bool Chest::Push(PGameObject go, int count, int pos)
{
  if (pos == -1)
  {
    for (auto &i : slots)
    {
      if (!i.obj)
      {
        i.obj = go;
        i.count = count;
        return true;
      }
    }
  }

  if (pos >= size)
    return false;
  slots[pos].obj = go;
  slots[pos].count = count;

  return true;
}

const ChestSlot Chest::GetFirst() const
{
  for (auto &cs : slots)
  {
    if (cs.obj)
      return cs;
  }
  return *slots.begin();
}

const ChestSlot Chest::GetFirst(int &pos) const
{
  int i = 0;
  for (auto &cs : slots)
  {
    if (cs.obj)
    {
      pos = i;
      return cs;
    }
    i++;
  }
  pos = -1;
  return *slots.begin();
}

ChestSlot Chest::PopFirst()
{
  for (auto &cs : slots)
  {
    if (cs.obj)
      return std::move(cs);
  }
  return std::move(*slots.begin());
}

ChestSlot Chest::PopSlot(int slot)
{
  if (slot == -1)
    return{};
  if (slot >= size)
    return{};
  return std::move(slots[slot]);
}

ChestSlot Chest::PopFirst(int &pos)
{
  int i = 0;
  for (auto &cs : slots)
  {
    if (cs.obj)
    {
      pos = i;
      return std::move(cs);
    }
    i++;
  }
  pos = -1;
  return std::move(*slots.begin());
}

ChestSlot Chest::PopSelected()
{
  return PopSlot(mSelected);
}

void Chest::PushSelected(ChestSlot cs)
{
  Push(cs.obj, cs.count, mSelected);
}

void Chest::Select(int slot)
{
  mSelected = slot;
}

int Chest::GetSelected()
{
  return mSelected;
}

void Chest::DrawGui()
{
  int jj = 666;
  int ii = 0;
  auto p = ImGui::GetWindowPos();
  for (auto &a : slots)
  {
    if (jj < 9)
      ImGui::SameLine(), jj++;
    else
      jj = 0;

    if (!a.obj)
    {
      auto &atl = TextureManager::Get().GetTexture("white");
      auto &tex = std::get<0>(atl);

      //some shit, remake stored uv to texture coord space
      auto uv = glm::vec2(std::get<1>(atl).x, std::get<1>(atl).y) / glm::vec2(tex->GetSize());
      auto uv2 = uv + glm::vec2(std::get<1>(atl).z, std::get<1>(atl).w) / glm::vec2(tex->GetSize());
      std::swap(uv.x, uv2.x);

      ImGui::ImageButton(reinterpret_cast<ImTextureID>(tex->GetId()), { 32,32 }, uv2, uv, -1, ii == mSelected ? ImVec4{ 1, 1, 1, 1 } : ImVec4{ 0,0,0,0 });
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


      ImGui::ImageButton(reinterpret_cast<ImTextureID>(tex->GetId()), { 32,32 }, uv2, uv, -1, ii == mSelected ? ImVec4{1, 1, 1, 1} : ImVec4{0,0,0,0});
      if (ImGui::IsItemHovered())
      {
        ImGui::SetTooltip("%s x%d", a.obj->GetId().get().c_str(), a.count);
      }
    }

    auto &dnd = DragNDrop::Get();
    if (ImGui::IsItemHovered())
    {
      if (a.obj && ImGui::IsMouseDragging(0) && !dnd.Busy())
        dnd.Drag(std::move(a));
      else if (dnd.Busy() && ImGui::IsMouseReleased(0))
        a = dnd.Drop();
    }
    ii++;
  }
}

