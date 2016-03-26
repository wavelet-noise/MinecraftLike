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
	if (!go)
		return false;

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
			else if (*i.obj == *go)
			{
				i.count += count;
				return true;
			}
		}
	}

	if (pos >= size)
		return false;

	if (*slots[pos].obj == *go)
	{
		slots[pos].count += count;
	}
	else
	{
		slots[pos].obj = go;
		slots[pos].count = count;
	}

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
			auto &uv4 = std::get<1>(atl);

			auto uv = glm::vec2(uv4.x, uv4.y);
			auto uv2 = glm::vec2(uv4.z, uv4.w);

			ImGui::ImageButton(reinterpret_cast<ImTextureID>(tex->GetId()), { 32,32 }, uv2, uv, -1, ii == mSelected ? ImVec4{ 1, 1, 1, 1 } : ImVec4{ 0, 0, 0, 0 });
		}
		else
		{
			auto &atl = TextureManager::Get().GetTexture(a.obj->GetId());
			auto &tex = std::get<0>(atl);
			auto &uv4 = std::get<1>(atl);

			auto uv = glm::vec2(uv4.x, uv4.y);
			auto uv2 = glm::vec2(uv4.z, uv4.w);

			ImGui::ImageButton(reinterpret_cast<ImTextureID>(tex->GetId()), { 32,32 }, uv2, uv, -1, ii == mSelected ? ImVec4{ 1, 1, 1, 1 } : ImVec4{ 0, 0, 0, 0 });
			auto draw_list = ImGui::GetWindowDrawList();

			if(a.count >= 100)
				draw_list->AddText(ImGui::GetItemBoxMax() - ImVec2(22, 13), ImGui::GetColorU32(ImGuiCol_Text), std::to_string(a.count).c_str());
			else if(a.count >= 10)
				draw_list->AddText(ImGui::GetItemBoxMax() - ImVec2(16, 13), ImGui::GetColorU32(ImGuiCol_Text), std::to_string(a.count).c_str());
			else
				draw_list->AddText(ImGui::GetItemBoxMax() - ImVec2(10, 13), ImGui::GetColorU32(ImGuiCol_Text), std::to_string(a.count).c_str());

			if (ImGui::IsItemHovered())
			{
				ImGui::SetTooltip("%s x%d\n%s", a.obj->GetId().get().c_str(), a.count, a.obj->GetDescription().c_str());
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

