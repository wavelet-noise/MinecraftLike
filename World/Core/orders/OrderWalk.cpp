#include "OrderWalk.h"
#include "../PositionAgent.h"
#include <glm/gtx/string_cast.inl>

std::string OrderWalk::to_string() const
{
	return (boost::format("OrderWalk: pos = %1%") % glm::to_string(pos)).str();
}

void OrderWalk::Perform(const GameObjectParams & params, PGameObject performer)
{
	auto c = performer->GetAgent<Creature>();
	if (c->path.empty())
		c->wishpos = pos;
	else
	{
		c->make_step(params);

		if (c->path.empty())
		{
			Done();
		}
	}
}
