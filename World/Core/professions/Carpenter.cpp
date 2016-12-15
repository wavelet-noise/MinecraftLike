#include "Carpenter.h"
#include <boost/locale/message.hpp>
#include <Core/orders/OrderCombined.h>

bool Carpenter::CanPeformOrder(POrder p)
{
	if(p->GetId() == Order::Idfor<OrderCombined>())
	{
		auto ord = std::static_pointer_cast<OrderCombined>(p);
		return ord->need_to_perform == GetId();
	}
	return false;
}

std::string Carpenter::Name()
{
	return boost::locale::translate("Carpenter");
}

std::string Carpenter::Description()
{
	return "some description";
}

void Carpenter::Perform(const GameObjectParams& params, PGameObject performer)
{
}

PProfession Carpenter::Clone()
{
	return std::make_shared<Carpenter>(*this);
}

float Carpenter::GetBaseCost() const
{
	return 10;
}
