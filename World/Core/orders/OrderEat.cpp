#include "OrderEat.h"
#include "../PositionAgent.h"
#include "../ChestSlot.h"
#include <glm/gtx/string_cast.hpp>
#include <core/World.h>
#include <core/Chest.h>

OrderEat::OrderEat(WBPos v, PGameObject i) : pos(v), item(i)
{
}

std::string OrderEat::to_string() const
{
	return (boost::format("OrderEat: pos = %1% id = %2%") % glm::to_string(pos) % item->GetId()).str() + Order::to_string();
}

void OrderEat::Perform(const GameObjectParams & params, PGameObject performer, float work)
{
	auto c = performer->GetAgent<Creature>();

	if (auto b = params.world->GetBlock(pos))
	{
		if (auto ch = b->GetAgent<Chest>())
		{
			auto poped = ch->PopByPredicate([&](const ChestSlot &o)->bool {
				return o.obj && o.obj->GetAgent<Food>();
			});
			if (poped.obj)
			{
				--poped.count;
				if (auto cal = performer->GetAgent<CalorieConsumer>())
				{
					if (cal->calorie <= 100 - poped.obj->GetAgent<Food>()->nutrition)
						cal->calorie += poped.obj->GetAgent<Food>()->nutrition;
				}

				if (poped.count > 0)
				{
					ch->Push(poped.obj, poped.count);
				}
			}
		}
	}

	Done();
}
