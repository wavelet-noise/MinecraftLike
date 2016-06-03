#pragma once
#include "../GameObject.h"
#include "OrderBus.h"

struct OrderSleep : public NumberedOrder<OrderSleep>
{
	OrderSleep(glm::vec3 v) : pos(v) {}

	std::string to_string() const override;

	glm::vec3 pos;

	glm::vec3 GetPos() const override
	{
		return pos;
	}

	bool IsEquals(const Order &rhs) override
	{
		if (rhs.GetId() != GetId())
			return false;

		const auto &o = static_cast<const OrderSleep &>(rhs);

		return o.pos == pos;
	}

	void Perform(const GameObjectParams & params, PGameObject performer) override;
};
