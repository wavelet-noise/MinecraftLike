#pragma once
#include "Core/GameObject.h"
#include "Core/orders/OrderBus.h"

struct OrderDig : NumberedOrder<OrderDig>
{
	OrderDig(glm::vec3 v);
	std::string to_string() const override;
	glm::vec3 pos;

	float Tiring() const override
	{
		return 0.3f;
	}

	glm::vec3 GetPos() const override
	{
		return pos;
	}

	bool IsEquals(const Order &rhs) override
	{
		if (rhs.GetId() != GetId())
			return false;

		const auto &o = static_cast<const OrderDig &>(rhs);

		return o.pos == pos;
	}

	void Perform(const GameObjectParams & params, PGameObject performer, float work = 0) override;
};