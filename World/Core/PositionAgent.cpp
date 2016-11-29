#include "PositionAgent.h"
#include <Core/orders/OrderBus.h>
#include <Core\GameObject.h>
#include <core\PhysicAgent.h>
#include <core\EventBus.h>
#include <core\World.h>
#include <imgui.h>
#include <glm\gtx\string_cast.hpp>
#include <fstream>
#include <core\Chest.h>
#include <queue>
#include <unordered_set>
#include <Render\ParticleSystem.h>
#include <Core/orders/OrderGet.h>
#include <Core/orders/OrderWander.h>
#include <Core/orders/OrderSleep.h>
#include <Core/orders/OrderTalk.h>
#include <Core/orders/OrderCraft.h>
#include <Core/orders/OrderEat.h>
#include <Core/orders/OrderDrop.h>
#include <gui/WindowDb.h>
#include "LiquidPipe.h"
#include <gui/WindowStorages.h>
#include "agents/ChestBlock.h"

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

void PositionAgent::DrawGui(float gt)
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
		size_t operator()(glm::vec3 const& v) const
		{
			size_t h1 = hash<int32_t>()(v.x);
			size_t h2 = hash<int32_t>()(v.y);
			size_t h3 = hash<int32_t>()(v.z);
			return ((h1 ^ (h2 << 1)) >> 1) ^ (h3 << 1);
		}
	};
}

int heuristic_cost_estimate(const glm::vec3 &start, const glm::vec3 &goal, World *w)
{
	if (start == goal)
		return 0;
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
	auto i = set.find(where);
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

	bool operator()(const glm::vec3 & t1, const glm::vec3 & t2) const
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

	while (!openSet.empty() && closedSet.size() < 1000 && openSet.size() < 2000)
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
		const auto &all_orders_queue = OrderBus::Get().orders;
		glm::vec3 nearest{ 99999 };
		POrder nearest_order;
		auto p = mParent->GetAgent<PositionAgent>();

		for (const auto &i : all_orders_queue)
		{
			if (!i->IsTaken() && glm::distance(p->Get(), i->GetPos()) < glm::distance(p->Get(), nearest))
			{
				if (auto tire = mParent->GetAgent<ActivityConsumer>())
				{
					if (tire->IsTired() && i->Tiring() > 0) // tired creatures cannot take tiring orders
						continue;

					if (auto prof = mParent->GetAgent<ProfessionPerformer>())
					{
						if (!prof->CanPeformOrder(i))
							continue;
					}
				}

				nearest = i->GetPos();
				nearest_order = i;
			}
		}

		if (auto ch = mParent->GetAgent<Chest>()) //empty pockets order
		{
			auto &i = ch->GetFirst();
			if (i.obj)
			{
				auto& storages = Storages::Get().List();
				if (!storages.empty())
				{
				    auto finded = std::find_if(storages.begin(), storages.end(), [&](const std::tuple<glm::vec3, PGameObject> & stor) -> bool
				    {
						return std::get<1>(stor)->GetAgent<Chest>()->CanPush(i.obj, i.count);
					});

					if (finded != storages.end())
					{
						auto tpos = std::get<0>(*finded);
						if (!storages.empty() && (glm::distance(p->Get(), tpos) < glm::distance(p->Get(), nearest)))
						{
							nearest_order = std::make_shared<OrderDrop>(std::get<0>(*finded), i.obj, i.count);
							nearest = tpos;
						}
					}
				}
			}
		}

		if (!c->personal.empty() && !nearest_order)
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

		{
			auto &ro = params.world->GetRecipeOrders();
			auto &storages = params.world->GetStorages();
			if (!storages.empty() && !ro.empty() && ro.begin()->elapsed > 0)
			{
				auto tpos = storages.begin()->first;
				if (!storages.empty() && (glm::distance(p->Get(), tpos) < glm::distance(p->Get(), nearest)))
				{
					auto i = *ro.begin();

					nearest_order = std::make_shared<OrderCraft>(storages.begin()->first, i.recipe, i.elapsed);
					nearest = tpos;
				}
			}
		}

		if (nearest_order)
		{
			nearest_order->Take();
			c->order = nearest_order;
			if (nearest_order->GetId() == Order::Idfor<OrderCraft>())
			{
				auto &ro = params.world->GetRecipeOrders();
				params.world->DoneRecipeOrder(ro.begin()->recipe);
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

void Creature::DiscardCurrentOrder()
{
	if (order->IsDone())
	{
		order = nullptr;
		path.clear();
		wishpos = {};
	}
	else
	{
		OrderBus::Get().IssueDelayedOrder(order);
		order->Drop();
		order = nullptr;
		path.clear();
		wishpos = {};
	}
}

void Creature::look_around(const GameObjectParams & params)
{
	if (auto m = mParent->GetAgent<Anatomic>())
	{
		for (int i = -3; i <= 3; i++)
			for (int j = -3; j <= 3; j++)
			{
				auto cpos = params.pos + glm::vec3{ i, j, 0 };
				if (auto b = params.world->GetBlock(cpos))
				{
					if (b->HasAgent<Chest>())
					{
						m->Interest("storage", cpos);
					}
				}
			}
	}
}

void Creature::make_step(const GameObjectParams & params)
{
	auto p = mParent->GetAgent<PositionAgent>();
	newpos = path.back();
	p->Set(newpos);
	path.pop_back();
	//p->Set(glm::mix(p->Get(), newpos, step_step * 2));

	//if (!path.empty())
	//{
	//	step_step += params.dt;

	//	if (step_step >= 0.5)
	//	{
	//		p->Set(newpos);
	//		newpos = path.back();
	//		look_around(params);
	//		path.pop_back();
	//		step_step = 0;
	//	}
	//}
}

void Creature::Update(const GameObjectParams & params)
{
	auto p = mParent->GetAgent<PositionAgent>();

	if (order && order->Tiring() > 0)
		if (auto tire = mParent->GetAgent<ActivityConsumer>())
		{
			tire->Tire(order->Tiring());
			if (tire->IsTired())
			{
				DiscardCurrentOrder();
			}
		}

	if (order) //TODO: move to order methods
	{
		order->Perform(params, mParent->shared_from_this());

		if (order)
		{
			if (path.empty())
			{
				path = Astar(glm::trunc(p->Get()), wishpos, params.world);
				if (path.empty() || order->IsDone())
				{
					DiscardCurrentOrder();
				}
			}
		}
	}
}

void Creature::DrawGui(float gt)
{
	if (Settings::Get().IsDebug())
	{
		if (order)
			ImGui::Text("Order: %s", order->to_string());
		else
			ImGui::Text("Order: none");

		if (ImGui::TreeNode("Self orders"))
		{
			if (personal.empty())
				ImGui::Text("empty");

			for (const auto &i : personal)
			{
				if (i)
					ImGui::Text(i->to_string().c_str());
			}
			ImGui::TreePop();
		}
	}
}

void Creature::Requirements()
{
	//REQUIRE(Chest);
}

void Creature::OnCreate(const GameObjectParams & params)
{
	uid = global_uid;
	global_uid++;

	params.world->RegisterCreature(mParent->shared_from_this());
}

void Creature::OnDestroy(const GameObjectParams& params)
{
	EventBus::Get().Publish<EventCreatureDeath>(mParent->shared_from_this());
}

void Creature::AddPersinal(POrder o)
{
	std::remove_if(personal.begin(), personal.end(), [&](const POrder &p)->bool {
		if (!p)
			return true;
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

void WalkingPossibility::DrawGui(float gt)
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

void CrawlingPossibility::DrawGui(float gt)
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

void WaterConsumer::DrawGui(float gt)
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
			auto tpos = storages.begin()->first;
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

void CalorieConsumer::DrawGui(float gt)
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

void Morale::DrawGui(float gt)
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

void Anatomic::DrawGui(float gt)
{
	if (ImGui::TreeNode("Mind"))
	{
		for (int i = 0; i < minds.size(); i++)
		{
			if (!(minds[i].mind.empty()))
				ImGui::Text(minds[i].mind.c_str());
		}
		ImGui::TreePop();
	}
}

void Anatomic::Think(const boost::format &s)
{
	Think(s.str());
}

void Anatomic::Think(const std::string &s)
{
	auto f = std::find_if(minds.begin(), minds.end(), [&](const Mind &m) -> bool { return m.mind == s; });
	if (f == minds.end())
	{
		minds.push_back({ s });
	}
}

void Anatomic::Interest(const std::string & s, glm::vec3 & p)
{
	interest_points[StringIntern(s)] = p;
	Think((boost::format("I see some %1% :|") % s).str());
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

void Named::DrawGui(float gt)
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
		auto npos = glm::ivec3(p->Get().x + rand() % 3 - 1, p->Get().y + rand() % 3 - 1, p->Get().z);
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

PAgent ActivityConsumer::Clone(GameObject * parent, const std::string & name)
{
	auto t = MakeAgent<ActivityConsumer>(*this);
	t->mParent = parent;
	return t;
}

void ActivityConsumer::Update(const GameObjectParams & params)
{
	activity -= params.dt / 5.f;

	if (activity <= 5)
	{
		if (auto c = mParent->GetAgent<Creature>())
		{
			c->AddPersinal(std::make_shared<OrderSleep>(mParent->GetAgent<PositionAgent>()->Get()));
		}
	}
}

void ActivityConsumer::DrawGui(float gt)
{
	if (Settings::Get().IsDebug())
	{
		ImGui::Text("Activity: %g", activity);
	}

	if (activity >= 50)
	{
		ImGui::TextColored({ 0,1,0.5,1 }, "Rested");
	}
	else if (activity >= 40)
	{
		ImGui::TextColored({ 0,1,0,1 }, "Not tired");
	}
	else if (activity >= 30)
	{
		ImGui::TextColored({ 0.5,1,0,1 }, "Slightly tired");
	}
	else if (activity >= 20)
	{
		ImGui::TextColored({ 1,1,0,1 }, "Moderately tired");
	}
	else
	{
		ImGui::TextColored({ 1,0,0,1 }, "Seriously tired");
		mParent->GetAgent<Anatomic>()->Think("I'm so tired :(");
	}
}

void ActivityConsumer::Tire(float t)
{
	activity -= t;
}

bool ActivityConsumer::IsTired() const
{
	return activity <= 20;
}

bool ActivityConsumer::IsRested() const
{
	return activity >= 100;
}

size_t Creature::global_uid = 0;

std::string Relationships::to_string() const
{
	return std::to_string(value);
}

PAgent Talker::Clone(GameObject * parent, const std::string & name)
{
	auto t = MakeAgent<Talker>(*this);
	t->mParent = parent;
	return t;
}

void Talker::Update(const GameObjectParams & params)
{
	if (auto c = mParent->GetAgent<Creature>())
	{
		auto p = mParent->GetAgent<PositionAgent>();
		auto in_cell = params.world->GetCreaturesAt(p->Get());
		if (in_cell.size() > 1)
			for (const auto &in : in_cell)
			{
				auto it = in_cell.begin();
				std::advance(it, rand() % in_cell.size());
				c->AddPersinal(std::make_shared<OrderTalk>(*it));
			}
	}
}

void Talker::DrawGui(float gt)
{
	if (ImGui::TreeNode("Relationships"))
	{
		for (auto &r : relationships)
		{
			ImGui::Text(r.second.with.c_str());
		}
		ImGui::TreePop();
	}
}

PAgent ProfessionPerformer::Clone(GameObject* parent, const std::string& name)
{
	auto t = MakeAgent<ProfessionPerformer>(*this);
	t->mParent = parent;
	return t;
}

void ProfessionPerformer::Update(const GameObjectParams& params)
{
	for (const auto &p : prof)
	{
		p->Perform(params, mParent->shared_from_this());
	}
}

void ProfessionPerformer::DrawGui(float gt)
{
	if (ImGui::TreeNode("Professions"))
	{
		for (const auto &p : prof)
		{
			ImGui::Text(p->Name().c_str());
		}
		ImGui::TreePop();
	}
}

bool ProfessionPerformer::CanPeformOrder(POrder o)
{
	for (const auto &p : prof)
	{
		if (p->CanPeformOrder(o))
			return true;
	}

	return false;
}

PAgent ChainDestruction::Clone(GameObject* parent, const std::string& name)
{
	auto t = MakeAgent<ChainDestruction>(*this);
	t->mParent = parent;
	return t;
}

void ChainDestruction::Update(const GameObjectParams& params)
{
}

void ChainDestruction::DrawGui(float gt)
{
}

void ChainDestruction::OnDestroy(const GameObjectParams& params)
{
	if (destroyed)
		return;

	destroyed = true;

	std::vector<glm::vec3> neib = {{ 0,  0, -1 }, {  0, 0, 1 },
	                               { 1,  0,  0 }, { -1, 0, 0 },
	                               { 0, -1,  0 }, {  0, 1, 0 }};

	for(const auto &n : neib)
	{
		if (auto ne = params.world->GetBlock(params.pos + n))
		{
			auto dest_it = std::find(destroys.begin(), destroys.end(), ne->GetId());
			if(dest_it != destroys.end())
				params.world->SetBlock(params.pos + n, nullptr);
		}
	}
}

void ChainDestruction::JsonLoad(const rapidjson::Value& val)
{
	JSONLOAD(NVP(destroys));
}

PAgent Workshop::Clone(GameObject* parent, const std::string& name)
{
	auto t = MakeAgent<Workshop>(*this);
	t->mParent = parent;
	return t;
}

void Workshop::Update(const GameObjectParams& params)
{
}

void Workshop::DrawGui(float gt)
{
	auto rec = DB::Get().GetMachineRecipe(mParent->GetId());
	for(const auto &a : rec)
	{
		a->DrawGui(gt);
	}
}

void Workshop::JsonLoad(const rapidjson::Value& val)
{
	JSONLOAD(NVP(machine));
}

//-------------------------------------------------------------------

PAgent EnergyProducer::Clone(GameObject* parent, const std::string& name)
{
	auto t = MakeAgent<EnergyProducer>(*this);
	t->mParent = parent;
	return t;
}

void EnergyProducer::Update(const GameObjectParams& params)
{
}

void EnergyProducer::DrawGui(float gt)
{
	if (Settings::Get().IsDebug())
	{
		ImGui::Text("EnergyProducer");
		ImGui::Text("producing %fv %fa", voltage, amperage);
		ImGui::Text("buffer %f / %f", buffer, buffer_size);
	}
}

void EnergyProducer::JsonLoad(const rapidjson::Value& val)
{
	JSONLOAD(NVP(amperage), NVP(voltage), NVP(buffer_size));
}

void EnergyProducer::ProduceEnergy(float power)
{
	buffer += power / voltage;

	if (buffer > buffer_size)
		buffer = buffer_size;
}

//-------------------------------------------------------------------

PAgent EnergyConsumer::Clone(GameObject* parent, const std::string& name)
{
	auto t = MakeAgent<EnergyConsumer>(*this);
	t->mParent = parent;
	return t;
}

void EnergyConsumer::Update(const GameObjectParams& params)
{
}

void EnergyConsumer::DrawGui(float gt)
{
	if (Settings::Get().IsDebug())
	{
		ImGui::Text("EnergyConsumer");
	}
}

void EnergyConsumer::JsonLoad(const rapidjson::Value& val)
{
}

//-------------------------------------------------------------------

PAgent EnergyWire::Clone(GameObject* parent, const std::string& name)
{
	auto t = MakeAgent<EnergyWire>(*this);
	t->mParent = parent;
	return t;
}

void EnergyWire::Update(const GameObjectParams& params)
{
}

void EnergyWire::DrawGui(float gt)
{
	if (Settings::Get().IsDebug())
	{
		ImGui::Text("EnergyWire");
	}
}

void EnergyWire::JsonLoad(const rapidjson::Value& val)
{
}

//-------------------------------------------------------------------

PAgent SteamGenerator::Clone(GameObject* parent, const std::string& name)
{
	auto t = MakeAgent<SteamGenerator>(*this);
	t->mParent = parent;
	return t;
}

void SteamGenerator::Update(const GameObjectParams& params)
{
	if(auto lp = mParent->GetAgent<LiquidPipe>())
	{
		if(lp->GetLiquidID() == StringIntern("material_steam"))
		{
			auto count = lp->GetLiquidCount();
			lp->SetLiquidCount(0);

			if(auto ep = mParent->GetAgent<EnergyProducer>())
			{
				ep->ProduceEnergy(count * efficiency);
			}
		}
	}
}

void SteamGenerator::DrawGui(float gt)
{
	if (Settings::Get().IsDebug())
	{
		ImGui::Text("SteamGenerator");
		ImGui::Text("efficiency %f%", efficiency * 100);
	}
}

void SteamGenerator::JsonLoad(const rapidjson::Value& val)
{
	JSONLOAD(NVP(efficiency));
}