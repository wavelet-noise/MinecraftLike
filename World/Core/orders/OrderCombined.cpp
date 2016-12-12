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
	return (boost::format("OrderCombined: count %1%") % orders.size()).str();
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

void OrderCombined::SetState(State __state)
{
	for (auto & or : orders)
	{
		or ->SetState(__state);
	}
}

Order::State OrderCombined::GetState() const
{
	for (const auto & or : orders)
	{
		if (!or ->IsDone())
			return or ->GetState();
	}
}

void OrderCombined::Rebuild(const GameObjectParams& params, PGameObject performer)
{
	state = State::Builded;
}

void OrderCombined::Approach(const GameObjectParams& params, PGameObject performer)
{
	state = State::Performing;
}

void OrderCombined::Perform(const GameObjectParams & params, PGameObject performer, float work)
{
	auto c = performer->GetAgent<Creature>();

	for (const auto & or : orders)
	{
		if (or ->IsCanceled())
			Cancel();

		if (!or->IsDone())
		{
			or->Update(params, performer, work);
			break;
		}
	}

	TODO ADD TEMPLATES AND SPAWNPOINT

	bool done = true;
	for (const auto & or : orders)
	{
		if (!or->IsDone())
			done = false;
	}

	if (done)
		Done();
}
