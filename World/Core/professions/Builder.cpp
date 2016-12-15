#include "Builder.h"
#include "Core/orders/OrderPlace.h"
#include "Core/orders/OrderFindAndPick.h"

bool Builder::CanPeformOrder(POrder p)
{
	if (p->GetId() == Order::Idfor<OrderPlace>())
		return true;

	if (p->GetId() == Order::Idfor<OrderFind>())
		return true;

	return false;
}

std::string Builder::Name()
{
	return "Builder";
}

std::string Builder::Description()
{
	return "some description";
}

void Builder::Perform(const GameObjectParams& params, PGameObject performer)
{
}

PProfession Builder::Clone()
{
	return std::make_shared<Builder>(*this);
}

float Builder::GetBaseCost() const
{
	return 10;
}