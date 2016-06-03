#pragma once
#include "GameObject.h"
#include "OrderBus.h"

struct OrderCraft : NumberedOrder<OrderCraft>
{
	OrderCraft(glm::vec3 v, PRecipe r, int c = 1) : pos(v), item(r), count(c)
	{
	}

	std::string to_string() const override;

	glm::vec3 pos;
	PRecipe item;
	int count = 1;

	float Tiring() const override
	{
		return 0.1f;
	}

	glm::vec3 GetPos() const override
	{
		return pos;
	}

	bool IsEquals(const Order &rhs) override
	{
		if (rhs.GetId() != GetId())
			return false;

		const auto &o = static_cast<const OrderCraft &>(rhs);

		return o.pos == pos;
	}

	void Perform(const GameObjectParams & params, PGameObject performer) override;
};
