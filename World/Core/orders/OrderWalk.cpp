#include "OrderWalk.h"
#include "../agents/PositionAgent.h"
#include <glm/gtx/string_cast.hpp>

std::string OrderWalk::to_string() const
{
	return (boost::format("OrderWalk: pos = %1%") % glm::to_string(pos)).str() + Order::to_string();
}

void OrderWalk::Perform(const GameObjectParams & params, PGameObject performer, float work)
{
	Done();
}
