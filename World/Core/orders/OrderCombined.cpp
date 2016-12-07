#include "OrderCombined.h"
#include "../PositionAgent.h"
#include <glm/gtx/string_cast.hpp>
#include <core/World.h>
#include <Core/Chest.h>

OrderCombined::OrderCombined()
{
}

std::string OrderCombined::to_string() const
{
	return (boost::format("OrderCombined: ")).str();
}

glm::vec3 OrderCombined::GetPos() const
{
	for(const auto & or : orders)
	{
		if (or ->IsDone())
			return or ->GetPos();
	}

	return {};
}

bool OrderCombined::IsEquals(const Order& rhs)
{
	if (rhs.GetId() != GetId())
		return false;

	const auto &o = static_cast<const OrderCombined &>(rhs);

	if (o.orders.size() != orders.size())
		return false;

	for (int i = 0; i < orders.size(); ++i)
	{
		if (!orders[i]->IsEquals(*o.orders[i]))
			return false;
	}

	return true;
}

void OrderCombined::PushOrder(POrder po)
{
	orders.push_back(po);
}

void OrderCombined::Perform(const GameObjectParams & params, PGameObject performer, float work)
{
	auto c = performer->GetAgent<Creature>();

	for (const auto & or : orders)
	{
		if (!or->IsDone())
		{
			or->Perform(params, performer, work);
		}

		if (or ->IsCanceled())
			Cancel();
	}

	bool done = true;
	for (const auto & or : orders)
	{
		if (!or ->IsDone())
			done = false;
	}

	if (done)
		Done();
}
