#include "Medic.h"

bool Medic::CanPeformOrder(POrder p)
{
	return false;
}

std::string Medic::Name()
{
	return "Medic";
}

std::string Medic::Description()
{
	return "some description";
}

void Medic::Perform(const GameObjectParams& params, PGameObject performer)
{
}

PProfession Medic::Clone()
{
	return std::make_shared<Medic>(*this);
}

float Medic::GetBaseCost() const
{
	return 10;
}