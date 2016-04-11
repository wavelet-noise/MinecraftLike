#include "PositionAgent.h"
#include <Core\OrderBus.h>
#include <Core\GameObject.h>
#include <core\PhysicAgent.h>
#include <core\EventBus.h>
#include <core\World.h>
#include <imgui.h>
#include <glm\gtx\string_cast.hpp>
#include <fstream>

PAgent PositionAgent::Clone(GameObject *parent, const std::string &name)
{
	auto t = MakeAgent<PositionAgent>(*this);
	t->mParent = parent;
	return t;
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

PAgent Controlable::Clone(GameObject * parent, const std::string & name)
{
	auto t = MakeAgent<Controlable>(*this);
	t->mParent = parent;
	return t;
}

void Controlable::Update(const GameObjectParams & params)
{
	auto c = mParent->GetAgent<Creature>();

	if (!c->order)
	{
		const auto &o = OrderBus::Get().orders;
		glm::vec3 nearest{ 99999 };
		POrder nearest_order;

		for (const auto &i : o)
		{
			auto p = mParent->GetAgent<PositionAgent>();


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
		auto p = mParent->GetAgent<PositionAgent>();
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
		ImGui::TextColored({ 1,0,0,1 }, "Dying of thirst");
		mParent->GetAgent<Anatomic>()->Think("I'm dying of thirst :(");
	}
	else if (water <= full / 3.f)
	{
		ImGui::TextColored({ 1,1,0,1 }, "Strong thirst");
		mParent->GetAgent<Anatomic>()->Think("I'm feel strong thirst :(");
	}
	else if (water <= full / 1.5f)
	{
		ImGui::TextColored({ 0.5,1,0,1 }, "Thirst");
		mParent->GetAgent<Anatomic>()->Think("I'm feel thirst :(");
	}
	else
	{
		ImGui::TextColored({ 0,1,0,1 }, "Full");
	}
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
		mParent->GetAgent<Anatomic>()->Think("I'm starwing :(");
	}
	else if (calorie <= full / 3.f)
	{
		ImGui::TextColored({ 1,1,0,1 }, "Very hungry");
		mParent->GetAgent<Anatomic>()->Think("I'm very hungry :(");
	}
	else if (calorie <= full / 1.5f)
	{
		ImGui::TextColored({ 0.5,1,0,1 }, "Hungry");
		mParent->GetAgent<Anatomic>()->Think("I'm hungry :(");
	}
	else
	{
		ImGui::TextColored({ 0,1,0,1 }, "Full");
	}
}

PAgent Morale::Clone(GameObject * parent, const std::string & name)
{
	auto t = MakeAgent<Morale>(*this);
	t->mParent = parent;
	return t;
}

void Morale::Update(const GameObjectParams & params)
{
}

void Morale::DrawGui()
{
	if (Settings::Get().IsDebug())
	{
		ImGui::Text("Morale: %g", morale);
	}

	if (morale >= 50)
	{
		ImGui::TextColored({ 0,1,0.5,1 }, "High morale");
	}
	else if (morale >= 40)
	{
		ImGui::TextColored({ 0,1,0,1 }, "Normal morale");
	}
	else if (morale >= 30)
	{
		ImGui::TextColored({ 0.5,1,0,1 }, "Slightly fatigued");
	}
	else if (morale >= 20)
	{
		ImGui::TextColored({ 1,1,0,1 }, "Moderately fatigued");
	}
	else
	{
		ImGui::TextColored({ 1,0,0,1 }, "Seriously fatigued");
		mParent->GetAgent<Anatomic>()->Think("Life is pointless :(");
	}
}

Anatomic::Anatomic()
{
	minds.resize(25);
}

PAgent Anatomic::Clone(GameObject * parent, const std::string & name)
{
	auto t = MakeAgent<Anatomic>(*this);
	t->mParent = parent;
	return t;
}

void Anatomic::Update(const GameObjectParams & params)
{
}

void Anatomic::Afterload(GameObject * parent)
{
}

void Anatomic::DrawGui()
{
	int j = 0;
	if (ImGui::TreeNode((boost::format("Mind##%1%") % j).str().c_str()))
	{
		for (int i = 0; i < minds.size(); i++)
		{
			if(!(minds[i].mind.empty()))
				ImGui::Text(minds[i].mind.c_str());
		}
		j++;
		ImGui::TreePop();
	}
}

void Anatomic::Think(const std::string &s)
{
	auto f = std::find_if(minds.begin(), minds.end(), [&](const Mind &m) -> bool { return m.mind == s; });
	if (f == minds.end())
	{
		minds.push_back({ s });
	}
}

PAgent Named::Clone(GameObject * parent, const std::string & name)
{
	auto t = MakeAgent<Named>(*this);
	t->mParent = parent;
	return t;
}

void Named::Update(const GameObjectParams & params)
{
}

std::vector<std::string> LoadNames(const std::string &name)
{
	std::ifstream file(name.c_str());
	std::vector<std::string> t;
	t.reserve(200);

	if (file.is_open())
	{
		std::string line = "";
		while (getline(file, line))
		{
			t.push_back(line);
		}
		file.close();
		return std::move(t);
	}
	else
	{
		LOG(fatal) << name << "missed";
		return{};
	}
}

void Named::OnCreate(const GameObjectParams & params)
{
	static std::vector<std::string> fname = []() -> auto { return LoadNames("data/loc/female_names.txt"); }(),
		                            mname = []() -> auto { return LoadNames("data/loc/male_names.txt"); }(),
		                            sname = []() -> auto { return LoadNames("data/loc/second_names.txt"); }();

	if (gender == 'f')
	{
		name = fname[rand() % fname.size()] + " " + sname[rand() % sname.size()];
	}
	else
	{
		name = mname[rand() % mname.size()] + " " + sname[rand() % sname.size()];
	}
}

void Named::DrawGui()
{
	ImGui::Text("Name: %s", name.c_str());
}
