#include "Cooker.h"
#include <boost/locale/message.hpp>

bool Cooker::CanPeformOrder(POrder p)
{
	return false;
}

std::string Cooker::Name()
{
	return boost::locale::translate("Cooker");
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
	return std::make_shared<Cooker>(*this);
}

float Cooker::GetBaseCost() const
{
	return 10;
}
