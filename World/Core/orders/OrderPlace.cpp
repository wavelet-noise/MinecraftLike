#include "OrderPlace.h"
#include <core/World.h>
#include <glm/gtx/string_cast.hpp>
#include <Core/agents/PositionAgent.h>
#include <Core/agents/Chest.h>
#include "OrderFindAndPick.h"

OrderPlace::OrderPlace(WBPos v, StringIntern i) : pos(v), item(i)
{
}

std::string OrderPlace::to_string() const
{
	return (boost::format("OrderPlace: pos = %1% id = %2%") % glm::to_string(pos) % item).str() + Order::to_string();
}

float OrderPlace::Tiring() const
{
	return 0.05f;
}

glm::vec3 OrderPlace::GetPos() const
{
	return pos;
}

bool OrderPlace::IsEquals(const Order& rhs)
{
	if (rhs.GetId() != GetId())
		return false;

	const auto &o = static_cast<const OrderPlace &>(rhs);

	return o.pos == pos && item == item;
}

void OrderPlace::Perform(const GameObjectParams & params, PGameObject performer, float work)
{
	auto c = performer->GetAgent<Creature>();

	if (params.world->IsAir(pos))
	{
		auto chest = performer->GetAgent<Chest>();
		if (chest)
		{
			auto has = chest->PopByPredicate([&](const ChestSlot & cs) -> bool
			{
				return cs.obj->GetId() == item;
			});

			if (has.obj)
			{
				params.world->SetBlock(pos, has.obj);

				has.count--;
				if (has.count > 0)
					chest->Push(has.obj->Clone(), has.count);

				Done();
			}
			else
			{
				Drop();
			}
		}
		else
		{
			Drop();
		}
	}
	else
	{
		Cancel("destination already contains block");
	}
}