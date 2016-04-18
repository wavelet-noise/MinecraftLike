#include "OrderBus.h"
#include <Core\EventBus.h>
#include <glm\gtx\string_cast.hpp>

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

void OrderBus::Update()
{
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
