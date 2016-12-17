#include "OrderBus.h"
#include <Core\EventBus.h>
#include <glm\gtx\string_cast.hpp>
#include <Core\agents\PositionAgent.h>
#include <core\GameObjectParams.h>
#include <core\agents\Ore.h>
#include <core\World.h>
#include <core\ChestSlot.h>
#include <Core\agents\Chest.h>
#include "OrderTools.h"

std::string Order::StateToString(State s)
{
	switch(s)
	{
	case State::Dropped:
		return "Dropped";
	case State::Builded:
		return "Builded";
	case State::Done:
		return "Done";
	case State::Initial:
		return "Initial";
	case State::Performing:
		return "Performing";
	default:
		return "ERROR";
	}
}

std::string Order::to_string() const
{
	return (boost::format(" (%1%%2%%3%)%4%") % (mTaken ? "T" : "_") % (mDone ? "D" : "_") % (mCanceled ? "C" : "_") % (reason.empty() ? "" : " [" + reason + "]")).str();
}

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

void Order::Rebuild(const GameObjectParams & params, PGameObject performer)
{
	path = ordertools::Astar(glm::trunc(performer->GetAgent<PositionAgent>()->Get()), GetPos(), params.world);
	if (path.empty())
		SetState(State::Dropped);
	else
		state = State::Builded;
}

void Order::Approach(const GameObjectParams& params, PGameObject performer)
{
	if(path.empty())
	{
		state = State::Performing;
		return;
	}
	auto p = performer->GetAgent<PositionAgent>();
	auto newpos = path.back();
	p->Set(newpos);
	path.pop_back();
}

void Order::Update(const GameObjectParams& params, PGameObject performer, float work)
{
	switch(state)
	{
	case State::Initial:
		Rebuild(params, performer);
		break;

	case State::Builded:
		Approach(params, performer);
		break;

	case State::Performing:
		Perform(params, performer, work);
		break;

	case State::Done:
	case State::Dropped:
		return;

	default:
		LOG(error) << to_string() << ": unknown order state (" << static_cast<int>(state) << ")";
		Cancel("unknown order state");
		break;
	}
}

void Order::SetState(State __state)
{
	LOG(trace) << to_string() << " New state: " << StateToString(__state);
	state = __state;
}

Order::State Order::GetState() const
{
	return state;
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
	SetState(State::Done);
}

void Order::Drop()
{
	EventBus::Get().Publish<EventOrderDrop>(shared_from_this());
	mTaken = false;
	SetState(State::Dropped);
}

void Order::Cancel(std::string __reason)
{
	EventBus::Get().Publish<EventOrderCancel>(shared_from_this());
	mDone = true;
	mCanceled = true;
	reason = __reason;
}

bool Order::IsTaken() const
{
	return mTaken;
}

bool Order::IsDone() const
{
	return mDone;
}

bool Order::IsCanceled() const
{
	return mCanceled;
}

size_t Order::Nextid()
{
	static size_t next_id(0);
	return next_id++;
}

float Order::Tiring() const
{
	return 0.f;
}
