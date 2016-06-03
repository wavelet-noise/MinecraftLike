#include "OrderSleep.h"
#include "PositionAgent.h"
#include <glm/gtx/string_cast.inl>

std::string OrderSleep::to_string() const
{
	return (boost::format("OrderSleep: pos = %1%") % glm::to_string(pos)).str();
}

void OrderSleep::Perform(const GameObjectParams & params, PGameObject performer)
{
	auto c = performer->GetAgent<Creature>();

	if (c->path.empty())
		c->wishpos = pos;
	else
	{
		c->make_step(params);

		if (c->path.empty())
		{
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
	}
}