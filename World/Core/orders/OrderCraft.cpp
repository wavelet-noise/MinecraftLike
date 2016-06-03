#include "OrderCraft.h"
#include <core/World.h>
#include <glm/gtx/string_cast.inl>
#include <core/PositionAgent.h>
#include <core/chest.h>

std::string OrderCraft::to_string() const
{
	return (boost::format("OrderCraft: pos = %1% count = %2% resilt[0] = %3%") % glm::to_string(pos) % count % (item ? item->output[0].id : StringIntern("ERROR"))).str();
}

void OrderCraft::Perform(const GameObjectParams & params, PGameObject performer)
{

	auto c = performer->GetAgent<Creature>();

	if (c->path.empty())
		c->wishpos = pos;
	else
	{
		c->make_step(params);

		if (c->path.empty())
		{
			if (auto b = params.world->GetBlock(pos))
			{
				if (auto c = b->GetAgent<Chest>())
				{
					if (!item->CraftIn(*c))
					{
						params.world->QueueRecipeOrder(item);
						params.world->DelayRecipeOrder(item);
					}
				}
			}

			Done();
		}
	}

}