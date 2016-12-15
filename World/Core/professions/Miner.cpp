#include "Miner.h"
#include "core/orders/OrderDig.h"

bool Miner::CanPeformOrder(POrder p)
{
	if (p->GetId() == Order::Idfor<OrderDig>())
		return true;

	return false;
}

std::string Miner::Name()
{
	return "Miner";
}

std::string Miner::Description()
{
	return "some description";
}

void Miner::Perform(const GameObjectParams& params, PGameObject performer)
{
}

PProfession Miner::Clone()
{
	return std::make_shared<Miner>(*this);
}

float Miner::GetBaseCost() const
{
	return 10;
}