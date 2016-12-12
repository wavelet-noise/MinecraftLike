#pragma once
#include <glm/detail/type_vec3.hpp>
#include <core/World.h>
#include <unordered_set>
#include <queue>

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

namespace ordertools
{
	inline int heuristic_cost_estimate(const glm::vec3 &start, const glm::vec3 &goal, World *w)
	{
		if (start == goal)
			return 0;
		if (auto b = w->GetBlock(start))
			return std::numeric_limits<int>().max();

		if (auto b = w->GetBlock(start - glm::vec3(0, 0, 1)))
			return glm::distance(start, goal);

		return std::numeric_limits<int>().max();
	}

	inline std::list<glm::vec3> reconstruct_path(const std::unordered_map<glm::vec3, glm::vec3> &cameFrom, const glm::vec3 &__current)
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

	inline int asearch(std::unordered_map<glm::vec3, int> &set, const glm::vec3 &where)
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

	inline std::list<glm::vec3> Astar(const glm::vec3 &start, const glm::vec3 &goal, World *w)
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

		LOG(error) << "destignation too far";
		return{};
	}
}