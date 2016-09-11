#include "OrderWander.h"
#include "../PositionAgent.h"
#include <glm/gtx/string_cast.inl>

std::string OrderWander::to_string() const
{
	return (boost::format("OrderWander: pos = %1%") % glm::to_string(pos)).str();
}

void OrderWander::Perform(const GameObjectParams & params, PGameObject performer, float work)
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
