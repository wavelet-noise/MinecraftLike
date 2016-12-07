#pragma once
#include "../GameObject.h"
#include "OrderBus.h"

struct OrderPick : NumberedOrder<OrderPick>
{
	OrderPick(WBPos v, PGameObject i, int c);

	std::string to_string() const override;

	glm::vec3 pos;
	PGameObject item;
	int count;

	glm::vec3 GetPos() const override;

	bool IsEquals(const Order& rhs) override;

	void Perform(const GameObjectParams & params, PGameObject performer, float work = 0) override;
};
