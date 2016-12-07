#include "OrderFindAndPick.h"
#include "../PositionAgent.h"
#include <glm/gtx/string_cast.hpp>
#include <core/World.h>
#include <Core/Chest.h>

OrderPick::OrderPick(WBPos v, PGameObject i, int c): pos(v), item(i), count(c)
{
}

std::string OrderPick::to_string() const
{
	return (boost::format("OrderPick: pos = %1%") % glm::to_string(pos)).str();
}

glm::vec3 OrderPick::GetPos() const
{
	return pos;
}

bool OrderPick::IsEquals(const Order& rhs)
{
	if (rhs.GetId() != GetId())
		return false;

	const auto &o = static_cast<const OrderPick &>(rhs);

	return o.pos == pos;
}

void OrderPick::Perform(const GameObjectParams & params, PGameObject performer, float work)
{
	auto c = performer->GetAgent<Creature>();

	if (c->path.empty())
		c->wishpos = pos;
	else
	{
		c->make_step(params);

		if (c->path.empty())
		{
			ChestSlot picked;
			if (auto b = params.world->GetBlock(pos))
			{
				if (auto ch = b->GetAgent<Chest>())
				{
					picked = ch->PopByPredicate([&](const ChestSlot & cs)->bool
					{
						return cs.obj == item && cs.count >= count;
					});
				}
			}

			if (picked.obj)
			{
				auto ch = performer->GetAgent<Chest>();
				auto canpush = ch->CanPush(picked.obj, count);
				if(canpush)
				{
					ch->Push(picked.obj, count);
					picked.count -= count;
					picked.obj = picked.obj->Clone();
					if(picked.count > 0)
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
	}
}
