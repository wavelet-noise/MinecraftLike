#include "OrderTalk.h"
#include <boost/format/format_fwd.hpp>
#include "../PositionAgent.h"

std::string OrderTalk::to_string() const
{
	return (boost::format("OrderTalk: creature = %1% duration = %2%") % person->GetAgent<Creature>()->uid % duration).str();
}

glm::vec3 OrderTalk::GetPos() const
{
	auto p = person->GetAgent<PositionAgent>();
	return p->Get();
}

void OrderTalk::Perform(const GameObjectParams & params, PGameObject performer)
{
	auto c = performer->GetAgent<Creature>();
	auto t = performer->GetAgent<Talker>();
	auto p = person->GetAgent<PositionAgent>();
	if (!t)
	{
		if (auto a = performer->GetAgent<Anatomic>())
		{
			std::string name = person->GetId();
			if (auto n = person->GetAgent<Named>())
				name = n->name;

			a->Think(boost::format("Want to talk with %1% but can't :|") % name);
		}

		return;
	}

	if (c->path.empty())
		c->wishpos = person->GetAgent<PositionAgent>()->Get();
	else
	{
		c->make_step(params);

		if (c->path.empty())
		{
			duration -= params.dt;

			if (duration <= 0)
			{
				if (auto c = person->GetAgent<Creature>())
				{
					auto i = t->relationships.find(c->uid);
					if (i == t->relationships.end())
					{
						Relationships rr;
						if (auto n = person->GetAgent<Named>())
						{
							rr.with = n->name;
						}

						t->relationships.insert({ c->uid, rr });
					}
				}

				if (auto a = performer->GetAgent<Anatomic>())
				{
					std::string name = person->GetId();
					if (auto n = person->GetAgent<Named>())
						name = n->name;

					if (person == performer)
						a->Think(boost::format("Just talking with myself :|"));
					else
						a->Think(boost::format("Just talking with %1% :)") % name);
				}

				Done();
			}
		}
	}
}