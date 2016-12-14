#include "OrderSleep.h"
#include <glm/gtx/string_cast.hpp>
#include <boost/format/format_fwd.hpp>
#include <Core/agents/PositionAgent.h>

std::string OrderSleep::to_string() const
{
	return (boost::format("OrderSleep: pos = %1%") % glm::to_string(pos)).str() + Order::to_string();
}

void OrderSleep::Perform(const GameObjectParams & params, PGameObject performer, float work)
{
	auto c = performer->GetAgent<Creature>();

	if (auto tire = performer->GetAgent<ActivityConsumer>())
	{
		tire->Tire(-.2f);
		if (tire->IsRested())
		{
			if (auto anat = performer->GetAgent<Anatomic>())
				anat->Think("Rested :)");
			Done();
		}
	}
	else
		Done();

}