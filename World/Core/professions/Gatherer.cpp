#include "Gatherer.h"

bool Gatherer::CanPeformOrder(POrder p)
{
	return false;
}

std::string Gatherer::Name()
{
	return "Gathering";
}

std::string Gatherer::Description()
{
	return "some description";
}

void Gatherer::Perform(const GameObjectParams& params, PGameObject performer)
{
}

PProfession Gatherer::Clone()
{
	return std::make_shared<Gatherer>(*this);
}

float Gatherer::GetBaseCost() const
{
	return 10;
}