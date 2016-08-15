#include "Cooker.h"

bool Cooker::CanPeformOrder(POrder p)
{
	return false;
}

std::string Cooker::Name()
{
	return "Cooker";
}

std::string Cooker::Description()
{
	return "some description";
}

void Cooker::Perform(const GameObjectParams& params, PGameObject performer)
{
}

PProfession Cooker::Clone()
{
	return std::make_shared<Cooker>();
}
