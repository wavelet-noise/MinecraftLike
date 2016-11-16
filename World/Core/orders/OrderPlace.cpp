#include "OrderPlace.h"
#include <core/World.h>
#include <glm/gtx/string_cast.hpp>
#include <core/PositionAgent.h>

OrderPlace::OrderPlace(WBPos v, PGameObject i) : pos(v), item(i)
{
}

std::string OrderPlace::to_string() const
{
	return (boost::format("OrderPlace: pos = %1% id = %2%") % glm::to_string(pos) % item->GetId()).str() + Order::to_string();
}

void OrderPlace::Perform(const GameObjectParams & params, PGameObject performer, float work)
{
	auto c = performer->GetAgent<Creature>();

	if (c->path.empty())
		c->wishpos = pos;
	else
	{
		c->make_step(params);

		if (c->path.empty())
		{
			params.world->SetBlock(pos, item);

			Done();
		}
	}
}