#pragma once
#include "../GameObject.h"
#include "OrderBus.h"

struct OrderCombined : NumberedOrder<OrderCombined>
{
	OrderCombined();

	std::string to_string() const override;

	std::vector<POrder> orders;

	glm::vec3 GetPos() const override;

	bool IsEquals(const Order& rhs) override;
	void PushOrder(POrder po);

	void Perform(const GameObjectParams & params, PGameObject performer, float work = 0) override;
};
