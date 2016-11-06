#include "OrderDrop.h"
#include <core/World.h>
#include <glm/gtx/string_cast.hpp>
#include "../ChestSlot.h"
#include "../PositionAgent.h"
#include <core/chest.h>

OrderDrop::OrderDrop(WBPos v, PGameObject i, int c) : pos(v), item(i), count(c)
{
}

std::string OrderDrop::to_string() const
{
	return (boost::format("OrderDrop: pos = %1% id = %2% count = %3%") % glm::to_string(pos) % item->GetId() % count).str();
}

void OrderDrop::Perform(const GameObjectParams & params, PGameObject performer, float work)
{
	auto c = performer->GetAgent<Creature>();

	if (c->path.empty())
		c->wishpos = pos;
	else
	{
		c->make_step(params);

		if (c->path.empty())
		{
			bool placed = false;
			if (auto b = params.world->GetBlock(pos))
			{
				if (auto ch = b->GetAgent<Chest>())
				{
					ch->Push(item, count);
					placed = true;
				}
			}

			if (placed)
			{
				auto ch = performer->GetAgent<Chest>();
				auto t_item = ch->PopByPredicate([&](const ChestSlot &cs)->bool {
					return cs.obj == this->item;
				});

				t_item.count -= count;
				if (t_item.count >= 1)
					ch->Push(t_item.obj, t_item.count);
			}

			Done();
		}
	}
}
