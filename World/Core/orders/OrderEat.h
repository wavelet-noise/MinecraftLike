#pragma once
#include "../GameObject.h"
#include "OrderBus.h"

struct GameObjectParams;

struct OrderEat : NumberedOrder<OrderEat>
{
	OrderEat(WBPos v, PGameObject i);
	std::string to_string() const override;
	WBPos pos;
	PGameObject item;

	glm::vec3 GetPos() const override
	{
		return pos;
	}

	bool IsEquals(const Order &rhs) override
	{
		if (rhs.GetId() != GetId())
			return false;

		const auto &o = static_cast<const OrderEat &>(rhs);

		return o.pos == pos && item == item;
	}

	void Perform(const GameObjectParams & params, PGameObject performer) override;
};
