#include "Chest.h"
#include <Serealize.h>
#include <imgui.h>
#include <Core\DB.h>
#include <Render\TextureManager.h>
#include <DragNDrop.h>

Chest::Chest()
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
		return false;
	}

	if (pos >= size)
		return false;

	if (slots[pos].obj && *slots[pos].obj == *go)
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

ChestSlot Chest::PopByPredicate(std::function<bool(const ChestSlot&)> pred)
{
	for (auto &cs : slots)
	{
		if (cs.obj)
		{
			if(pred(cs))
				return std::move(cs);
		}
	}

	return {};
}

ChestSlot Chest::GetByPredicate(std::function<bool(const ChestSlot&)> pred)
{
	for (auto &cs : slots)
	{
		if (cs.obj)
		{
			if (pred(cs))
				return cs;
		}
	}

	return{};
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
	ImGui::Text("Storage");
	int jj = 666;
	int ii = 0;
	auto p = ImGui::GetWindowPos();
	for (auto &a : slots)
	{
		if (jj < 9)
			ImGui::SameLine(), jj++;
		else
			jj = 0;

		a.DrawGui(ii == mSelected);

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

PAgent InputChest::Clone(GameObject * parent, const std::string & name)
{
	auto t = MakeAgent<InputChest>(*this);
	t->mParent = parent;
	return t;
}

PAgent OutputChest::Clone(GameObject * parent, const std::string & name)
{
	auto t = MakeAgent<OutputChest>(*this);
	t->mParent = parent;
	return t;
}
