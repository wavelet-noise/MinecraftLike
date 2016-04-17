#include "PositionAgent.h"
#include <Core\OrderBus.h>
#include <Core\GameObject.h>
#include <core\PhysicAgent.h>
#include <core\EventBus.h>
#include <core\OrderBus.h>
#include <core\World.h>
#include <imgui.h>
#include <glm\gtx\string_cast.hpp>
#include <fstream>
#include <core\Chest.h>
#include <queue>
#include <unordered_set>
#include <Render\ParticleSystem.h>

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

namespace std
{
	template <>
	struct hash<glm::vec3>
	{
		std::size_t operator()(glm::vec3 const& v) const
		{
			std::size_t h1 = std::hash<int32_t>()(v.x);
			std::size_t h2 = std::hash<int32_t>()(v.y);
			std::size_t h3 = std::hash<int32_t>()(v.z);
			return ((h1 ^ (h2 << 1)) >> 1) ^ (h3 << 1);
		}
	};
}


int heuristic_cost_estimate(const glm::vec3 &start, const glm::vec3 &goal, World *w)
{
	if (auto b = w->GetBlock(goal))
	{
		return std::numeric_limits<int>().max();
	}
	//else if (auto b = w->GetBlock(goal - glm::vec3(0,0,-1)))
	return glm::distance(start, goal);

	return std::numeric_limits<int>().max();
}

std::list<glm::vec3> reconstruct_path(const std::unordered_map<glm::vec3, glm::vec3> &cameFrom, const glm::vec3 &__current)
{
	glm::vec3 current = __current;
	std::list<glm::vec3> total_path = { current };
	auto f = cameFrom.find(current);
	while (true)
	{
		if (f == cameFrom.end())
			break;

		current = f->second;
		total_path.push_back(current);
		f = cameFrom.find(current);
		ParticleSystem::Get().Add(current + glm::vec3(0, 0, 0.5), StringIntern("car"), 5, 1.f, Color(0, 255, 0, 1));
	}

	return total_path;
}

static const glm::vec3 nieb[] = {
	{ -1, -1, -1 },
	{ 0, -1, -1 },
	{ 1, -1, -1 },
	{ -1,  0, -1 },
	{ 0,  0, -1 },
	{ 1,  0, -1 },
	{ -1,  1, -1 },
	{ 0,  1, -1 },
	{ 1,  1, -1 },

	{ -1, -1,  0 },
	{ 0, -1,  0 },
	{ 1, -1,  0 },
	{ -1,  0,  0 },
	//{  0,  0,  0 },
	{ 1,  0,  0 },
	{ -1,  1,  0 },
	{ 0,  1,  0 },
	{ 1,  1,  0 },

	{ -1, -1,  1 },
	{ 0, -1,  1 },
	{ 1, -1,  1 },
	{ -1,  0,  1 },
	{ 0,  0,  1 },
	{ 1,  0,  1 },
	{ -1,  1,  1 },
	{ 0,  1,  1 },
	{ 1,  1,  1 }
};

std::list<glm::vec3> Astar(const glm::vec3 &start, const glm::vec3 &goal, World *w)
{
	// The set of nodes already evaluated.
	std::unordered_set<glm::vec3> closedSet = {};
	// The set of currently discovered nodes still to be evaluated.
	// Initially, only the start node is known.
	std::unordered_set<glm::vec3> openSet = { start };
	// For each node, which node it can most efficiently be reached from.
	// If a node can be reached from many nodes, cameFrom will eventually contain the
	// most efficient previous step.
	std::unordered_map<glm::vec3, glm::vec3> cameFrom = {};

	// For each node, the cost of getting from the start node to that node.
	std::unordered_map<glm::vec3, int> gScore;
	// The cost of going from start to start is zero.
	gScore[start] = 0;
	// For each node, the total cost of getting from the start node to the goal
	// by passing by that node. That value is partly known, partly heuristic.
	std::unordered_map<glm::vec3, int> fScore;
	// For the first node, that value is completely heuristic.
	fScore[start] = heuristic_cost_estimate(start, goal, w);
	if (fScore[start] == std::numeric_limits<int>().max())
		return{};

	while (!openSet.empty() && closedSet.size() < 4096 && openSet.size() < 4096)
	{
		auto &current = *openSet.begin();// the node in openSet having the lowest fScore[] value
		if (current == goal)
		{
			return reconstruct_path(cameFrom, current);
		}

		openSet.erase(current);
		closedSet.insert(current);
		for (auto &n : nieb)
		{
			auto neighbor = current + n;
			if (closedSet.find(neighbor) != closedSet.end())
				continue; // Ignore the neighbor which is already evaluated.

						  // The distance from start to a neighbor
			auto tentative_gScore = gScore[current] + glm::distance(current, neighbor);
			if (openSet.find(neighbor) == openSet.end()) // Discover a new node
				openSet.insert(neighbor);
			else if (tentative_gScore >= gScore[neighbor])
				continue; // This is not a better path.

						  // This path is the best until now. Record it!
			cameFrom[neighbor] = current;
			gScore[neighbor] = tentative_gScore;
			fScore[neighbor] = gScore[neighbor] + heuristic_cost_estimate(neighbor, goal, w);

			ParticleSystem::Get().Add(neighbor + glm::vec3(0, 0, 0.5), StringIntern("car"), 5, 1.f, Color(fScore[neighbor] / glm::distance(start, goal) * 255, 0, 0, 1));
		}
	}

	return{};
}

void Controlable::Update(const GameObjectParams & params)
{
	auto c = mParent->GetAgent<Creature>();

	if (!c->order || c->order->GetId() == Order::Idfor<OrderWander>())
	{
		const auto &o = OrderBus::Get().orders;
		glm::vec3 nearest{ 99999 };
		POrder nearest_order;
		auto p = mParent->GetAgent<PositionAgent>();

		for (const auto &i : o)
		{
			if (!i->IsTaken() && i->GetId() == Order::Idfor<OrderDig>())
			{
				const auto &d = std::static_pointer_cast<OrderDig>(i);
				if (glm::distance(p->Get(), d->pos) < glm::distance(p->Get(), nearest))
				{
					nearest = d->pos;
					nearest_order = d;
				}
			}

			if (!i->IsTaken() && i->GetId() == Order::Idfor<OrderGet>())
			{
				const auto &d = std::static_pointer_cast<OrderGet>(i);
				if (glm::distance(p->Get(), d->pos) < glm::distance(p->Get(), nearest))
				{
					nearest = d->pos;
					nearest_order = d;
				}
			}

			if (!i->IsTaken() && i->GetId() == Order::Idfor<OrderWalk>())
			{
				const auto &d = std::static_pointer_cast<OrderWalk>(i);
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
			if (nearest_order->GetId() == Order::Idfor<OrderDig>())
			{
				const auto &d = std::static_pointer_cast<OrderDig>(nearest_order);
				Astar(glm::trunc(p->Get()), d->pos + glm::vec3(0, 0, 1), params.world);
			}
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

		if (order->GetId() == Order::Idfor<OrderDig>())
		{
			auto &pos = std::static_pointer_cast<OrderDig>(order)->pos;
			p->Set(glm::mix(p->Get(), pos, params.dt));

			if (glm::distance(std::static_pointer_cast<OrderDig>(order)->pos, p->Get()) < 1)
			{
				params.world->SetBlock(std::static_pointer_cast<OrderDig>(order)->pos, nullptr);
				order->Done();
				order = nullptr;
			}
		}
		else if (order->GetId() == Order::Idfor<OrderGet>())
		{
			auto &ord = std::static_pointer_cast<OrderGet>(order);
			auto &pos = ord->pos;
			p->Set(glm::mix(p->Get(), pos, params.dt));

			if (glm::distance(std::static_pointer_cast<OrderGet>(order)->pos, p->Get()) < 1)
			{
				params.world->Replace(ord->pos, ord->item);

				auto p = mParent->GetAgent<Chest>();
				p->Push(ord->item);

				order->Done();
				order = nullptr;
			}
		}
		else if (order->GetId() == Order::Idfor<OrderWalk>())
		{
			auto &pos = std::static_pointer_cast<OrderWalk>(order)->pos;
			p->Set(glm::mix(p->Get(), pos, params.dt));

			if (glm::distance(pos, p->Get()) < 1)
			{
				order->Done();
				order = nullptr;
			}
		}
		else if (order->GetId() == Order::Idfor<OrderWander>())
		{
			auto &pos = std::static_pointer_cast<OrderWander>(order)->pos;
			p->Set(glm::mix(p->Get(), pos, params.dt));

			if (glm::distance(pos, p->Get()) < 1)
			{
				order->Done();
				order = nullptr;
			}
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
			if (!(minds[i].mind.empty()))
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

PAgent DeathDrop::Clone(GameObject * parent, const std::string & name)
{
	auto t = MakeAgent<DeathDrop>(*this);
	t->mParent = parent;
	return t;
}

void DeathDrop::Update(const GameObjectParams & params)
{
}

void DeathDrop::OnDestroy(const GameObjectParams & params)
{
	auto i = DB::Get().Create(id);
	params.world->Place(params.pos, i);
	OrderBus::Get().IssueOrder(std::make_shared<OrderGet>(params.pos, i));
}

void DeathDrop::JsonLoad(const rapidjson::Value & val)
{
	JSONLOAD(NVP(id));
}

PAgent Wander::Clone(GameObject * parent, const std::string & name)
{
	auto t = MakeAgent<Wander>(*this);
	t->mParent = parent;
	return t;
}

void Wander::Update(const GameObjectParams & params)
{
	if (auto c = mParent->GetAgent<Creature>())
	{
		if (!c->order)
		{
			auto p = mParent->GetAgent<PositionAgent>();
			c->order = std::make_shared<OrderWander>(p->Get() + glm::vec3((rand() % 60 - 30) / 10.f, (rand() % 60 - 30) / 10.f, 2));
		}
	}
}
