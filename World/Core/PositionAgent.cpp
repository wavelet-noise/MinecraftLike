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
#include <queue>
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
	if (auto b = w->GetBlock(start))
	{
		return std::numeric_limits<int>().max();
	}
	else if (auto b = w->GetBlock(start - glm::vec3(0, 0, 1)))
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
		//ParticleSystem::Get().Add(current + glm::vec3(0, 0, 0.5), StringIntern("car"), 1, 1.f, Color(0, 255, 0, 1));
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

int asearch(std::unordered_map<glm::vec3, int> &set, const glm::vec3 &where)
{
	auto &i = set.find(where);
	if (i != set.end())
	{
		return i->second;
	}

	set[where] = std::numeric_limits<int>().max();
	return std::numeric_limits<int>().max();
}

class CompareVec {
public:

	static glm::vec3 goal;

	bool operator()(const glm::vec3 & t1, const glm::vec3 & t2)
	{
		return glm::distance(t1, goal) > glm::distance(t2, goal);
	}
};

glm::vec3 CompareVec::goal;

std::list<glm::vec3> Astar(const glm::vec3 &start, const glm::vec3 &goal, World *w)
{
	// The set of nodes already evaluated.
	std::unordered_set<glm::vec3> closedSet = {};
	// The set of currently discovered nodes still to be evaluated.
	// Initially, only the start node is known.
	CompareVec::goal = goal;
	std::priority_queue<glm::vec3, std::vector<glm::vec3>, CompareVec> openSet;
	openSet.push(start);
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
	fScore[start] = glm::distance(start, goal);

	while (!openSet.empty() && closedSet.size() < 100 && openSet.size() < 200)
	{
		glm::vec3 current = openSet.top();// the node in openSet having the lowest fScore[] value
		if (current == goal)
		{
			return reconstruct_path(cameFrom, current);
		}

		openSet.pop();
		closedSet.insert(current);
		for (auto &n : nieb)
		{
			auto neighbor = current + n;
			if (closedSet.find(neighbor) != closedSet.end())
				continue; // Ignore the neighbor which is already evaluated.

			// The distance from start to a neighbor
			auto tentative_gScore = asearch(gScore, current) + glm::distance(current, neighbor);

			auto heu = heuristic_cost_estimate(neighbor, goal, w);
			if (heu == std::numeric_limits<int>().max())
				continue; //no way

			openSet.push(neighbor);
			if (tentative_gScore >= asearch(gScore, neighbor))
				continue; // This is not a better path.

						  // This path is the best until now. Record it!
			cameFrom[neighbor] = current;
			gScore[neighbor] = tentative_gScore;
			fScore[neighbor] = tentative_gScore + heu;

			//ParticleSystem::Get().Add(neighbor + glm::vec3(0, 0, 0.5), StringIntern("car"), 1, 1.f, Color(fScore[neighbor] / glm::distance(start, goal) * 255, 0, 0, 1));
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
			if (!i->IsTaken())
			{
				if (i->GetId() == Order::Idfor<OrderDig>())
				{
					const auto &d = std::static_pointer_cast<OrderDig>(i);
					if (glm::distance(p->Get(), d->pos) < glm::distance(p->Get(), nearest))
					{
						nearest = d->pos;
						nearest_order = d;
					}
				}
				else if (i->GetId() == Order::Idfor<OrderGet>())
				{
					const auto &d = std::static_pointer_cast<OrderGet>(i);
					if (glm::distance(p->Get(), d->pos) < glm::distance(p->Get(), nearest))
					{
						nearest = d->pos;
						nearest_order = d;
					}
				}
				else if (i->GetId() == Order::Idfor<OrderWalk>())
				{
					const auto &d = std::static_pointer_cast<OrderWalk>(i);
					if (glm::distance(p->Get(), d->pos) < glm::distance(p->Get(), nearest))
					{
						nearest = d->pos;
						nearest_order = d;
					}
				}
				else if (i->GetId() == Order::Idfor<OrderPlace>())
				{
					const auto &d = std::static_pointer_cast<OrderPlace>(i);
					if (glm::distance(p->Get(), glm::vec3(d->pos)) < glm::distance(p->Get(), nearest))
					{
						nearest = d->pos;
						nearest_order = d;
					}
				}
				else if (i->GetId() == Order::Idfor<OrderDrop>())
				{
					const auto &d = std::static_pointer_cast<OrderDrop>(i);
					if (glm::distance(p->Get(), glm::vec3(d->pos)) < glm::distance(p->Get(), nearest))
					{
						nearest = d->pos;
						nearest_order = d;
					}
				}
			}
		}

		if (auto ch = mParent->GetAgent<Chest>())
		{
			auto &i = ch->GetFirst();
			if (i.obj)
			{
				auto &storages = params.world->GetStorages();
				if (!storages.empty())
				{
					auto tpos = storages.begin()->first + glm::vec3(0, 0, 1);
					if (!storages.empty() && (glm::distance(p->Get(), tpos) < glm::distance(p->Get(), nearest)))
					{
						auto i = ch->PopFirst();
						nearest_order = std::make_shared<OrderDrop>(storages.begin()->first + glm::vec3(0, 0, 1), i.obj, i.count);
						nearest = tpos;
					}
				}
			}
		}

		auto c = mParent->GetAgent<Creature>();
		if (!c->personal.empty())
		{
			if (!c->personal.begin()->get())
			{
				c->personal.pop_front();
			}
			else
			if (glm::distance(p->Get(), (*c->personal.begin())->GetPos()) < glm::distance(p->Get(), nearest))
			{
				nearest = (*c->personal.begin())->GetPos();
				nearest_order = c->personal.front();
				c->personal.pop_front();
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

void Creature::Clear()
{
	order->Done();
	order = nullptr;
	path.clear();
	wishpos = {};
}

void Creature::Update(const GameObjectParams & params)
{
	if (order)
	{
		auto p = mParent->GetAgent<PositionAgent>();

		if (order->GetId() == Order::Idfor<OrderDig>())
		{
			auto &pos = std::static_pointer_cast<OrderDig>(order)->pos;

			if (path.empty())
				wishpos = pos + glm::vec3(0, 0, 1);
			else
			{
				p->Set(path.back());
				path.pop_back();

				if (path.empty())
				{
					params.world->SetBlock(pos, nullptr);
					Clear();
				}
			}
		}
		else if (order->GetId() == Order::Idfor<OrderGet>())
		{
			auto &ord = std::static_pointer_cast<OrderGet>(order);
			auto &pos = ord->pos;

			if (path.empty())
				wishpos = pos;
			else
			{
				p->Set(path.back());
				path.pop_back();

				if (path.empty())
				{
					params.world->Replace(pos, ord->item);

					auto p = mParent->GetAgent<Chest>();
					p->Push(ord->item);

					Clear();
				}
			}
		}
		else if (order->GetId() == Order::Idfor<OrderWalk>())
		{
			auto &pos = std::static_pointer_cast<OrderWalk>(order)->pos;

			if (path.empty())
				wishpos = pos;
			else
			{
				p->Set(path.back());
				path.pop_back();

				if (path.empty())
				{
					Clear();
				}
			}
		}
		else if (order->GetId() == Order::Idfor<OrderWander>())
		{
			auto &pos = std::static_pointer_cast<OrderWander>(order)->pos;

			if (path.empty())
				wishpos = pos;
			else
			{
				p->Set(path.back());
				path.pop_back();

				if (path.empty())
				{
					Clear();
				}
			}
		}
		else if (order->GetId() == Order::Idfor<OrderPlace>())
		{
			auto &ord = std::static_pointer_cast<OrderPlace>(order);
			auto &pos = ord->pos;

			if (path.empty())
				wishpos = pos;
			else
			{
				p->Set(path.back());
				path.pop_back();

				if (path.empty())
				{
					params.world->SetBlock(pos, ord->item);

					Clear();
				}
			}
		}
		else if (order->GetId() == Order::Idfor<OrderDrop>())
		{
			auto &ord = std::static_pointer_cast<OrderDrop>(order);
			auto &pos = ord->pos;

			if (path.empty())
				wishpos = pos;
			else
			{
				p->Set(path.back());
				path.pop_back();

				if (path.empty())
				{
					bool placed = false;
					if (auto b = params.world->GetBlock(pos - glm::ivec3(0, 0, 1)))
					{
						if (auto ch = b->GetAgent<Chest>())
						{
							ch->Push(ord->item, ord->count);
							placed = true;
						}
					}

					if (!placed)
					{
						//
					}

					Clear();
				}
			}
		}
		else if (order->GetId() == Order::Idfor<OrderEat>())
		{
			auto &ord = std::static_pointer_cast<OrderEat>(order);
			auto &pos = ord->pos;

			if (path.empty())
				wishpos = pos;
			else
			{
				p->Set(path.back());
				path.pop_back();

				if (path.empty())
				{
					if (auto b = params.world->GetBlock(pos - glm::ivec3(0, 0, 1)))
					{
						if (auto ch = b->GetAgent<Chest>())
						{
							auto poped = ch->PopByPredicate([&](const ChestSlot &o)->bool {
								return o.obj && o.obj->GetAgent<Food>();
							});
							if (poped.obj)
							{
								--poped.count;
								if (auto cal = mParent->GetAgent<CalorieConsumer>())
								{
									if (cal->calorie <= 100 - poped.obj->GetAgent<Food>()->nutrition)
									cal->calorie += poped.obj->GetAgent<Food>()->nutrition;
								}

								if (poped.count > 0)
								{
									ch->Push(poped.obj, poped.count);
								}
							}
						}
					}

					Clear();
				}
			}
		}

		if (path.empty() && order)
		{
			path = Astar(glm::trunc(p->Get()), wishpos, params.world);
			if (path.empty())
			{
				Clear();
				return;
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

		static int j = 0;
		if (ImGui::TreeNode((boost::format("Self orders##%1%") % j).str().c_str()))
		{
			j++;
			for (const auto &i : personal)
			{
				ImGui::Text(i->to_string().c_str());
			}
			ImGui::TreePop();
		}
	}
}

void Creature::AddPersinal(POrder o)
{
	std::remove_if(personal.begin(), personal.end(), [&](const POrder &p)->bool {
		return p->GetId() == o->GetId();
	});

	personal.push_front(o);
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

	if (want_to_eat)
		if (auto c = mParent->GetAgent<Creature>())
		{
			auto &storages = params.world->GetStorages();
			auto tpos = storages.begin()->first + glm::vec3(0, 0, 1);
			if (storages.size() > 0)
			{
				auto ch = storages.begin()->second->GetAgent<Chest>();
				auto i = ch->GetByPredicate([](const ChestSlot &o)->bool
				{
					return o.obj && o.obj->GetAgent<Food>();
				});

				if (i.obj)
				{
					c->AddPersinal(std::make_shared<OrderEat>(tpos, i.obj));
					want_to_eat = false;
				}
			}
		}
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
		want_to_eat = true;
	}
	else if (calorie <= full / 3.f)
	{
		ImGui::TextColored({ 1,1,0,1 }, "Very hungry");
		mParent->GetAgent<Anatomic>()->Think("I'm very hungry :(");
		want_to_eat = true;
	}
	else if (calorie <= full / 1.5f)
	{
		ImGui::TextColored({ 0.5,1,0,1 }, "Hungry");
		mParent->GetAgent<Anatomic>()->Think("I'm hungry :(");
		want_to_eat = true;
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
		auto p = mParent->GetAgent<PositionAgent>();
		auto npos = p->Get() + glm::vec3((rand() % 60 - 30) / 10.f, (rand() % 60 - 30) / 10.f, 0);
		if (params.world->IsWalkable(npos))
		{
			c->AddPersinal(std::make_shared<OrderWander>(npos));
		}
	}
}

PAgent Food::Clone(GameObject * parent, const std::string & name)
{
	auto t = MakeAgent<Food>(*this);
	t->mParent = parent;
	return t;
}

void Food::JsonLoad(const rapidjson::Value & val)
{
	JSONLOAD(NVP(nutrition));
}
