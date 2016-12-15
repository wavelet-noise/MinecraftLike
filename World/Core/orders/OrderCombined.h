#pragma once
#include "../GameObject.h"
#include "OrderBus.h"
#include <Core/professions/Profession.h>

struct OrderCombined : NumberedOrder<OrderCombined>
{
	OrderCombined();

	std::string to_string() const override;

	std::vector<POrder> orders;
	size_t need_to_perform = 0; //PProfession id

	glm::vec3 GetPos() const override;

	bool IsEquals(const Order& rhs) override;
	void PushOrder(POrder po);

	void SetState(State __state) override;
	State GetState() const override;

	void Rebuild(const GameObjectParams & params, PGameObject performer) override;
	void Approach(const GameObjectParams & params, PGameObject performer) override;

	void Perform(const GameObjectParams & params, PGameObject performer, float work = 0) override;
};
