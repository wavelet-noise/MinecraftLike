#include "OrderOperate.h"
#include <core/World.h>
#include <glm/gtx/string_cast.hpp>
#include <core/PositionAgent.h>

OrderOperate::OrderOperate(WBPos v, POperatable i) : pos(v), mechanism(i)
{
}

std::string OrderOperate::to_string() const
{
	return (boost::format("OrderOperate: pos = %1% id = %2%") % glm::to_string(pos) % mechanism->Parent()->GetId()).str();
}

void OrderOperate::Done()
{
	Order::Done();
	mechanism->EndOperate();
}

void OrderOperate::Perform(const GameObjectParams & params, PGameObject performer, float work)
{
	auto c = performer->GetAgent<Creature>();

	if (c->path.empty())
		c->wishpos = pos;
	else
	{
		c->make_step(params);

		if (c->path.empty())
		{
			if (!mechanism->HasWork())
				Done();

			mechanism->DoWork();
		}
	}
}