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
	JSONLOAD(NVP(columns), NVP(size), NVP(volume));
	slots.resize(size);
}

PAgent Chest::Clone(GameObject * parent, const std::string &name)
{
	auto t = MakeAgent<Chest>(*this);
	t->slots.resize(size);
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
				if (max_volume != -1)
				{
					if (volume + count > max_volume)
						return false;

					volume += count;
				}
				i.obj = go;
				i.count = count;
				return true;
			}
			else if (*i.obj == *go)
			{
				if (max_volume != -1)
				{
					if (volume + count > max_volume)
						return false;

					volume += count;
				}
				i.count += count;
				return true;
			}
		}
		return false;
	}

	if (pos >= size)
		return false;

	if (max_volume != -1)
	{
		if (volume + count > max_volume)
			return false;

		volume += count;
	}

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

bool Chest::CanPush(PGameObject go, int count, int pos) const
{
	if (!go)
		return false;

	if (pos == -1)
	{
		for (auto &i : slots)
		{
			if (!i.obj)
			{
				if (max_volume != -1)
				{
					if (volume + count > max_volume)
						return false;

					//volume += count;
				}
				//i.obj = go;
				//i.count = count;
				return true;
			}
			else if (*i.obj == *go)
			{
				if (max_volume != -1)
				{
					if (volume + count > max_volume)
						return false;

					//volume += count;
				}
				//i.count += count;
				return true;
			}
		}
		return false;
	}

	if (pos >= size)
		return false;

	if (max_volume != -1)
	{
		if (volume + count > max_volume)
			return false;

		//volume += count;
	}

	if (slots[pos].obj && *slots[pos].obj == *go)
	{
		//slots[pos].count += count;
	}
	else
	{
		//slots[pos].obj = go;
		//slots[pos].count = count;
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
		{
			if (max_volume != -1)
			{
				volume -= cs.count;
			}
			return std::move(cs);
		}
	}
	return{};
}

ChestSlot Chest::PopSlot(int slot)
{
	if (slot == -1)
		return{};
	if (slot >= size)
		return{};

	if (max_volume != -1)
	{
		volume -= slots[slot].count;
	}

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
			if (max_volume != -1)
			{
				volume -= cs.count;
			}
			return std::move(cs);
		}
		i++;
	}
	pos = -1;
	return{};
}

ChestSlot Chest::PopByPredicate(std::function<bool(const ChestSlot&)> pred)
{
	for (auto &cs : slots)
	{
		if (cs.obj)
		{
			if (pred(cs))
			{
				if (max_volume != -1)
				{
					volume -= cs.count;
				}
				return std::move(cs);
			}
		}
	}

	return{};
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

bool Chest::DrawGui(float gt)
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

		a.DrawGui();

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
	if (max_volume != -1)
		ImGui::Text("Volume %d of %d", volume, max_volume);

	return true;
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
