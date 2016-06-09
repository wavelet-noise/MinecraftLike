#include "OrderBus.h"
#include <Core\EventBus.h>
#include <glm\gtx\string_cast.hpp>
#include <Core\PositionAgent.h>
#include <core\GameObjectParams.h>
#include <core\Ore.h>
#include <core\World.h>
#include <core\ChestSlot.h>
#include <core\Chest.h>

void OrderBus::IssueDelayedOrder(POrder ord)
{
	if (delayed_orders.size() >= 10000)
	{
		LOG(error) << "too much delayed orders";
		delayed_orders.clear();
	}

	for (const auto o : delayed_orders)
	{
		if (o->IsEquals(*ord))
			return;
	}
	delayed_orders.push_back(ord);
}

void OrderBus::IssueOrder(POrder ord)
{
	if (orders.size() >= 10000)
	{
		LOG(error) << "too much orders";
		orders.clear();
	}

	for (const auto o : orders)
	{
		if (o->IsEquals(*ord))
			return;
	}
	orders.push_back(ord);
}

void OrderBus::Clear()
{
	orders.clear();
}

void OrderBus::Update(float dt)
{
	static float ro_delay = 0;
	ro_delay += dt;
	if (ro_delay >= 5)
	{
		ro_delay = 0;
		orders.insert(orders.begin(), delayed_orders.begin(), delayed_orders.end());
		delayed_orders.clear();
	}

	for (auto i = orders.begin(); i != orders.end(); ++i)
	{
		if ((*i)->IsTaken())
		{
			orders_taken.push_back(*i);
			orders.erase(i);
			break;
		}
	}

	for (auto i = orders_taken.begin(); i != orders_taken.end(); ++i)
	{
		if ((*i)->IsDone())
		{
			orders_taken.erase(i);
			break;
		}
	}
}

std::string Order::to_string() const
{
	return (boost::format("Order: id = %1%") % GetId()).str();
}

void Order::Take()
{
	EventBus::Get().Publish<EventOrderStart>(shared_from_this());
	mTaken = true;
}

void Order::Done()
{
	EventBus::Get().Publish<EventOrderDone>(shared_from_this());
	mDone = true;
}

void Order::Drop()
{
	mTaken = false;
}

float Order::Tiring() const
{
	return 0.f;
}

void Order::Perform(const GameObjectParams & params, PGameObject performer)
{
}