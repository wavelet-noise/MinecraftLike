#include "OrderBus.h"

void OrderBus::IssueOrder(std::shared_ptr<Order> ord)
{
	for (const auto o : orders)
	{
		if (o->IsEquals(*ord))
			return;
	}
	orders.push_back(ord);
}

OrderDig::OrderDig(glm::vec3 v) : pos(v)
{
}
