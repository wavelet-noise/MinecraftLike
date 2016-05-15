#include "OrderBus.h"
#include <Core\EventBus.h>
#include <glm\gtx\string_cast.hpp>

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

OrderDig::OrderDig(glm::vec3 v) : pos(v)
{
}

std::string OrderDig::to_string() const
{
	return (boost::format("OrderDig: pos = %1%") % glm::to_string(pos)).str();
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

OrderGet::OrderGet(glm::vec3 v, PGameObject i) : pos(v), item(i)
{
}

std::string OrderGet::to_string() const
{
	return (boost::format("OrderGet: pos = %1% id = %2%") % glm::to_string(pos) % item->GetId()).str();
}

OrderWalk::OrderWalk(glm::vec3 v) : pos(v)
{
}

std::string OrderWalk::to_string() const
{
	return (boost::format("OrderWalk: pos = %1%") % glm::to_string(pos)).str();
}

OrderWander::OrderWander(glm::vec3 v) : pos(v)
{
}

std::string OrderWander::to_string() const
{
	return (boost::format("OrderWander: pos = %1%") % glm::to_string(pos)).str();
}

OrderPlace::OrderPlace(WBPos v, PGameObject i) : pos(v), item(i)
{
}

std::string OrderPlace::to_string() const
{
	return (boost::format("OrderPlace: pos = %1% id = %2%") % glm::to_string(pos) % item->GetId()).str();
}

OrderDrop::OrderDrop(WBPos v, PGameObject i, int c) : pos(v), item(i), count(c)
{
}

std::string OrderDrop::to_string() const
{
	return (boost::format("OrderDrop: pos = %1% id = %2% count = %3%") % glm::to_string(pos) % item->GetId() % count).str();
}

OrderEat::OrderEat(WBPos v, PGameObject i) : pos(v), item(i)
{
}

std::string OrderEat::to_string() const
{
	return (boost::format("OrderEat: pos = %1% id = %2%") % glm::to_string(pos) % item->GetId()).str();
}

OrderCraft::OrderCraft(glm::vec3 v, PRecipe r, int c) : pos(v), item(r), count(c)
{
}

std::string OrderCraft::to_string() const
{
	return (boost::format("OrderCraft: pos = %1% count = %2% resilt[0] = %3%") % glm::to_string(pos) % count % (item ? item->output[0].id : StringIntern("ERROR"))).str();
}

std::string OrderSleep::to_string() const
{
	return (boost::format("OrderSleep: pos = %1%") % glm::to_string(pos)).str();
}
