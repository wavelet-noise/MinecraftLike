#include "OrderBus.h"
#include <Core\EventBus.h>
#include <glm\gtx\string_cast.hpp>
#include <Core\PositionAgent.h>
#include <core\GameObjectParams.h>
#include <core\Ore.h>
#include <core\World.h>
#include <core\ChestSlot.h>
#include <core\Chest.h>

void OrderBus::IssueDelayedOrder(POrder ord)
{
	if (delayed_orders.size() >= 10000)
	{
		LOG(error) << "too much delayed orders";
		delayed_orders.clear();
	}

	for (const auto o : delayed_orders)
	{
		if (o->IsEquals(*ord))
			return;
	}
	delayed_orders.push_back(ord);
}

void OrderBus::IssueOrder(POrder ord)
{
	if (orders.size() >= 10000)
	{
		LOG(error) << "too much orders";
		orders.clear();
	}

	for (const auto o : orders)
	{
		if (o->IsEquals(*ord))
			return;
	}
	orders.push_back(ord);
}

void OrderBus::Clear()
{
	orders.clear();
}

void OrderBus::Update(float dt)
{
	static float ro_delay = 0;
	ro_delay += dt;
	if (ro_delay >= 5)
	{
		ro_delay = 0;
		orders.insert(orders.begin(), delayed_orders.begin(), delayed_orders.end());
		delayed_orders.clear();
	}

	for (auto i = orders.begin(); i != orders.end(); ++i)
	{
		if ((*i)->IsTaken())
		{
			orders_taken.push_back(*i);
			orders.erase(i);
			break;
		}
	}

	for (auto i = orders_taken.begin(); i != orders_taken.end(); ++i)
	{
		if ((*i)->IsDone())
		{
			orders_taken.erase(i);
			break;
		}
	}
}

OrderDig::OrderDig(glm::vec3 v) : pos(v)
{
}

std::string OrderDig::to_string() const
{
	return (boost::format("OrderDig: pos = %1%") % glm::to_string(pos)).str();
}

void OrderDig::Perform(const GameObjectParams & params, PGameObject performer)
{
	auto c = performer->GetAgent<Creature>();

	if (c->path.empty())
		c->wishpos = pos;
	else
	{
		c->make_step(params);

		if (c->path.empty())
		{
			bool remove = true;
			if (auto orb = params.world->GetBlock(pos))
			{
				if (auto ore = orb->GetAgent<Ore>())
				{
					remove = false;
					auto ch = performer->GetAgent<Chest>();
					auto cs = ore->DigSome();
					ch->Push(cs.obj, cs.count);

					if (ore->Expire())
						remove = true;
				}
			}

			if (remove)
			{
				params.world->SetBlock(pos, nullptr);
				Done();
			}
		}
	}
}

std::string Order::to_string() const
{
	return (boost::format("Order: id = %1%") % GetId()).str();
}

void Order::Take()
{
	EventBus::Get().Publish<EventOrderStart>(shared_from_this());
	mTaken = true;
}

void Order::Done()
{
	EventBus::Get().Publish<EventOrderDone>(shared_from_this());
	mDone = true;
}

void Order::Drop()
{
	mTaken = false;
}

float Order::Tiring() const
{
	return 0.f;
}

void Order::Perform(const GameObjectParams & params, PGameObject performer)
{
}

OrderGet::OrderGet(glm::vec3 v, PGameObject i) : pos(v), item(i)
{
}

std::string OrderGet::to_string() const
{
	return (boost::format("OrderGet: pos = %1% id = %2%") % glm::to_string(pos) % item->GetId()).str();
}

OrderPlace::OrderPlace(WBPos v, PGameObject i) : pos(v), item(i)
{
}

std::string OrderPlace::to_string() const
{
	return (boost::format("OrderPlace: pos = %1% id = %2%") % glm::to_string(pos) % item->GetId()).str();
}

OrderDrop::OrderDrop(WBPos v, PGameObject i, int c) : pos(v), item(i), count(c)
{
}

std::string OrderDrop::to_string() const
{
	return (boost::format("OrderDrop: pos = %1% id = %2% count = %3%") % glm::to_string(pos) % item->GetId() % count).str();
}

OrderEat::OrderEat(WBPos v, PGameObject i) : pos(v), item(i)
{
}

std::string OrderEat::to_string() const
{
	return (boost::format("OrderEat: pos = %1% id = %2%") % glm::to_string(pos) % item->GetId()).str();
}

std::string OrderSleep::to_string() const
{
	return (boost::format("OrderSleep: pos = %1%") % glm::to_string(pos)).str();
}

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

					if(person == performer)
						a->Think(boost::format("Just talking with myself :|"));
					else
						a->Think(boost::format("Just talking with %1% :)") % name);
				}

				Done();
			}
		}
	}
}

std::string OrderWalk::to_string() const
{
	return (boost::format("OrderWalk: pos = %1%") % glm::to_string(pos)).str();
}

std::string OrderWander::to_string() const
{
	return (boost::format("OrderWander: pos = %1%") % glm::to_string(pos)).str();
}

std::string OrderCraft::to_string() const
{
	return (boost::format("OrderCraft: pos = %1% count = %2% resilt[0] = %3%") % glm::to_string(pos) % count % (item ? item->output[0].id : StringIntern("ERROR"))).str();
}
