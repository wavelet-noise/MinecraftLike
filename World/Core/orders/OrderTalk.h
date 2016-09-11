#pragma once
#include "../GameObject.h"
#include "OrderBus.h"

struct OrderTalk : NumberedOrder<OrderTalk>
{
	OrderTalk(PGameObject p) : person(p)
	{
	}

	std::string to_string() const override;

	PGameObject person;
	float duration = 2.f;

	glm::vec3 GetPos() const override;

	bool IsEquals(const Order &rhs) override
	{
		if (rhs.GetId() != GetId())
			return false;

		const auto &o = static_cast<const OrderTalk &>(rhs);

		return o.person == person;
	}

	void Perform(const GameObjectParams & params, PGameObject performer, float work = 0) override;
};
