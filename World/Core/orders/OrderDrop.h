#pragma once
#include "../GameObject.h"
#include "OrderBus.h"

struct OrderDrop : NumberedOrder<OrderDrop>
{
	OrderDrop(WBPos v, PGameObject i, int c);
	std::string to_string() const override;
	WBPos pos;
	PGameObject item;
	int count;

	glm::vec3 GetPos() const override
	{
		return pos;
	}

	bool IsEquals(const Order &rhs) override
	{
		if (rhs.GetId() != GetId())
			return false;

		const auto &o = static_cast<const OrderDrop &>(rhs);

		return o.pos == pos && item == item && count == o.count;
	}

	void Perform(const GameObjectParams & params, PGameObject performer, float work = 0) override;
};
