#include "Mason.h"
#include <boost/locale/message.hpp>

bool Mason::CanPeformOrder(POrder p)
{
	return false;
}

std::string Mason::Name()
{
	return boost::locale::translate("Mason");
}

std::string Mason::Description()
{
	return "some description";
}

void Mason::Perform(const GameObjectParams& params, PGameObject performer)
{
}

PProfession Mason::Clone()
{
	return std::make_shared<Mason>(*this);
}

float Mason::GetBaseCost() const
{
	return 10;
}
