#include "OrderCraft.h"
#include <core/World.h>
#include <glm/gtx/string_cast.hpp>
#include <Core/agents/PositionAgent.h>
#include <Core/agents/Chest.h>

std::string OrderCraft::to_string() const
{
	return (boost::format("OrderCraft: pos = %1% count = %2% resilt[0] = %3%") %
		glm::to_string(pos) %
		count %
		(item ? item->output[0].id : StringIntern("ERROR"))).str() +
		Order::to_string();
}

void OrderCraft::Perform(const GameObjectParams & params, PGameObject performer, float work)
{

	auto c = performer->GetAgent<Creature>();

	duration_passed += work;
	if (item->duration > duration_passed)
		return;

	if (!item->CraftIn(*performer->GetAgent<Chest>(), count, tag_map))
		Drop();

	Done();

}