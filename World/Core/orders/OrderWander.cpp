#include "OrderWander.h"
#include "../PositionAgent.h"
#include <glm/gtx/string_cast.hpp>

std::string OrderWander::to_string() const
{
	return (boost::format("OrderWander: pos = %1%") % glm::to_string(pos)).str() + Order::to_string();
}

void OrderWander::Perform(const GameObjectParams & params, PGameObject performer, float work)
{
	Done();
}
