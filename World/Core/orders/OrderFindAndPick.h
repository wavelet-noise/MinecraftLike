#pragma once
#include "../GameObject.h"
#include "OrderBus.h"

struct OrderFind : NumberedOrder<OrderFind>
{
	OrderFind(StringIntern __id, int c);

	std::string to_string() const override;

	StringIntern id;
	int count;
	WBPos pos;

	glm::vec3 GetPos() const override;

	bool IsEquals(const Order& rhs) override;

	virtual void Rebuild(const GameObjectParams & params, PGameObject performer);
	void Perform(const GameObjectParams & params, PGameObject performer, float work = 0) override;
};