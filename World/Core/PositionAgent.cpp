#include "PositionAgent.h"
#include <Core\OrderBus.h>
#include <Core\GameObject.h>
#include <core\PhysicAgent.h>
#include <core\EventBus.h>
#include <core\World.h>
#include <imgui.h>
#include <glm\gtx\string_cast.hpp>

PositionAgent::PositionAgent()
	: Agent(nullptr, "PositionAgent", "")
{
}

PositionAgent::PositionAgent(GameObject *parent, const std::string &name)
	: Agent(parent, "PositionAgent", name)
{
}


PositionAgent::PositionAgent(const PositionAgent &object, GameObject *parent, const std::string &name)
	: Agent(parent, "PositionAgent", name)
{

}

PositionAgent::~PositionAgent()
{
}

PAgent PositionAgent::Clone(GameObject *parent, const std::string &name)
{
	return MakeAgent<PositionAgent>(*this, parent, name);
}

void PositionAgent::JsonLoad(const rapidjson::Value & val)
{
	Agent::JsonLoad(val);
}

void PositionAgent::Update(const GameObjectParams &params)
{

}

void PositionAgent::DrawGui()
{
	if (Settings::Get().IsDebug())
	{
		ImGui::Text("Position: %s", glm::to_string(mPos).c_str());
	}
}

Controlable::Controlable()
	: Agent(nullptr, "Controlable", "")
{
}

PAgent Controlable::Clone(GameObject * parent, const std::string & name)
{
	auto t = MakeAgent<Controlable>(*this);
	t->mParent = parent;
	return t;
}

void Controlable::Update(const GameObjectParams & params)
{
	auto c = mParent->GetFromFullName<Creature>("Creature");

	if (!c->order)
	{
		const auto &o = OrderBus::Get().orders;
		glm::vec3 nearest{ 99999 };
		POrder nearest_order;

		for (const auto &i : o)
		{
			auto p = mParent->GetFromFullName<PositionAgent>("PositionAgent");


			if (!i->IsTaken() && i->GetId() == Order::Idfor<OrderDig>())
			{
				const auto &d = std::static_pointer_cast<OrderDig>(i);
				if (glm::distance(p->Get(), d->pos) < glm::distance(p->Get(), nearest))
				{
					nearest = d->pos;
					nearest_order = d;
				}
			}
		}

		if (nearest_order)
		{
			nearest_order->Take();
			c->order = nearest_order;
		}
	}
}

Creature::Creature()
	: Agent(nullptr, "Creature", "")
{
}

PAgent Creature::Clone(GameObject * parent, const std::string & name)
{
	auto t = MakeAgent<Creature>(*this);
	t->mParent = parent;
	return t;
}

void Creature::Update(const GameObjectParams & params)
{
	if (order)
	{
		auto p = mParent->GetFromFullName<PositionAgent>("PositionAgent");
		auto &pos = std::static_pointer_cast<OrderDig>(order)->pos;
		p->Set(glm::mix(p->Get(), pos, params.dt));

		if (glm::distance(std::static_pointer_cast<OrderDig>(order)->pos, p->Get()) < 1)
		{
			params.world->SetBlock(std::static_pointer_cast<OrderDig>(order)->pos, nullptr);
			order->Done();
			order = nullptr;
		}
	}
}

void Creature::DrawGui()
{
	if (Settings::Get().IsDebug())
	{
		if (order)
			ImGui::Text("Order: %s", order->to_string());
		else
			ImGui::Text("Order: none");
	}
}

WalkingPossibility::WalkingPossibility()
	: Agent(nullptr, "WalkingPossibility", "")
{
}

PAgent WalkingPossibility::Clone(GameObject * parent, const std::string & name)
{
	auto t = MakeAgent<WalkingPossibility>(*this);
	t->mParent = parent;
	return t;
}

void WalkingPossibility::Update(const GameObjectParams & params)
{
}

void WalkingPossibility::DrawGui()
{
	ImGui::Text("Can walk");
}

CrawlingPossibility::CrawlingPossibility()
	: Agent(nullptr, "CrawlingPossibility", "")
{
}

PAgent CrawlingPossibility::Clone(GameObject * parent, const std::string & name)
{
	auto t = MakeAgent<CrawlingPossibility>(*this);
	t->mParent = parent;
	return t;
}

void CrawlingPossibility::Update(const GameObjectParams & params)
{
}

void CrawlingPossibility::DrawGui()
{
	ImGui::Text("Can crawl");
}

WaterConsumer::WaterConsumer()
	: Agent(nullptr, "WaterConsumer", "")
{
}

PAgent WaterConsumer::Clone(GameObject * parent, const std::string & name)
{
	auto t = MakeAgent<WaterConsumer>(*this);
	t->mParent = parent;
	return t;
}

void WaterConsumer::Update(const GameObjectParams & params)
{
	water -= params.dt / 10.f;
}

void WaterConsumer::DrawGui()
{
	if (Settings::Get().IsDebug())
	{
		ImGui::Text("Water: %g", water);
	}

	if (water <= 0)
	{
		ImGui::TextColored({ 1,0,0,1 }, "Dry");
	}
	else if (water <= full / 3.f)
	{
		ImGui::TextColored({ 1,1,0,1 }, "Strong thirst");
	}
	else if (water <= full / 1.5f)
	{
		ImGui::TextColored({ 0.5,1,0,1 }, "Thirst");
	}
	else
	{
		ImGui::TextColored({ 0,1,0,1 }, "Full");
	}
}

CalorieConsumer::CalorieConsumer()
	: Agent(nullptr, "CalorieConsumer", "")
{
}

PAgent CalorieConsumer::Clone(GameObject * parent, const std::string & name)
{
	auto t = MakeAgent<CalorieConsumer>(*this);
	t->mParent = parent;
	return t;
}

void CalorieConsumer::Update(const GameObjectParams & params)
{
	calorie -= params.dt / 10.f;
}

void CalorieConsumer::DrawGui()
{
	if (Settings::Get().IsDebug())
	{
		ImGui::Text("Calorie: %g", calorie);
	}

	if (calorie <= 0)
	{
		ImGui::TextColored({ 1,0,0,1 }, "Starwing");
	}
	else if (calorie <= full / 3.f)
	{
		ImGui::TextColored({ 1,1,0,1 }, "Very hungry");
	}
	else if (calorie <= full / 1.5f)
	{
		ImGui::TextColored({ 0.5,1,0,1 }, "Hungry");
	}
	else
	{
		ImGui::TextColored({ 0,1,0,1 }, "Full");
	}
}
