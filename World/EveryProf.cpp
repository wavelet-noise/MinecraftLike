#include "EveryProf.h"

bool EveryProf::CanPeformOrder(POrder p)
{
	return true;
}

std::string EveryProf::Name()
{
	return "Anything";
}

std::string EveryProf::Description()
{
	return "some description";
}

void EveryProf::Perform(const GameObjectParams& params, PGameObject performer)
{
}

PProfession EveryProf::Clone()
{
	return std::make_shared<EveryProf>();
}
