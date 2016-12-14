#include "OrderFindAndPick.h"
#include <Core/agents/PositionAgent.h>
#include <glm/gtx/string_cast.hpp>
#include <core/World.h>
#include <Core/agents/Chest.h>

OrderFind::OrderFind(StringIntern i, int c) : id(i), count(c)
{
}

std::string OrderFind::to_string() const
{
	return (boost::format("OrderFind: id = %1%") % id).str();
}

glm::vec3 OrderFind::GetPos() const
{
	return pos;
}

bool OrderFind::IsEquals(const Order& rhs)
{
	if (rhs.GetId() != GetId())
		return false;

	const auto &o = static_cast<const OrderFind &>(rhs);

	return o.id == id && o.count == count;
}

void OrderFind::Rebuild(const GameObjectParams& params, PGameObject performer)
{
	auto & stor = params.world->GetStorages();
	PGameObject go;
	for (const auto & s : stor)
	{
		auto cs = s.second->GetAgent<Chest>()->GetByPredicate([&](const ChestSlot & chest_slot)->bool
		{
			return chest_slot.obj->GetId() == id;
		});

		if (cs.obj)
		{
			go = cs.obj;
			pos = s.first;
			break;
		}
	}

	if (go)
		Order::Rebuild(params, performer);
	else
		SetState(State::Dropped);
}

void OrderFind::Perform(const GameObjectParams & params, PGameObject performer, float work)
{
	auto c = performer->GetAgent<Creature>();

	ChestSlot picked;
	if (auto b = params.world->GetBlock(pos))
	{
		if (auto ch = b->GetAgent<Chest>())
		{
			picked = ch->PopByPredicate([&](const ChestSlot & cs)->bool
			{
				return cs.obj->GetId() == id && cs.count >= count;
			});
		}
	}

	if (picked.obj)
	{
		auto ch = performer->GetAgent<Chest>();
		auto canpush = ch->CanPush(picked.obj, count);
		if (canpush)
		{
			ch->Push(picked.obj, count);
			picked.count -= count;
			picked.obj = picked.obj->Clone();
			if (picked.count > 0)
				params.world->GetBlock(pos)->GetAgent<Chest>()->Push(picked.obj, picked.count);
			Done();
		}
		else
		{
			params.world->GetBlock(pos)->GetAgent<Chest>()->Push(picked.obj, picked.count);
			Drop();
		}

		Done();
	}
	else
	{
		Drop();
	}
}
