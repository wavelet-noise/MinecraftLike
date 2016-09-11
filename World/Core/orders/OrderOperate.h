#pragma once
#include "../GameObject.h"
#include "OrderBus.h"
#include <Core/Operatable.h>

struct OrderOperate : NumberedOrder<OrderOperate>
{
	OrderOperate(WBPos v, POperatable i);
	std::string to_string() const override;
	WBPos pos;
	POperatable mechanism;

	float Tiring() const override
	{
		return 0.05f;
	}

	glm::vec3 GetPos() const override
	{
		return pos;
	}

	void Done() override;

	bool IsEquals(const Order &rhs) override
	{
		if (rhs.GetId() != GetId())
			return false;

		const auto &o = static_cast<const OrderOperate &>(rhs);

		return o.pos == pos && mechanism == mechanism;
	}

	void Perform(const GameObjectParams & params, PGameObject performer, float work = 0) override;
};
