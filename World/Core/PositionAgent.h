#pragma once


#include "Agent.h"
#include "..\tools\CoordSystem.h"
#include <Core\orders\OrderBus.h>
#include <unordered_map>
#include <boost\circular_buffer.hpp>
#include <deque>
#include <boost\format.hpp>
#include <Profession.h>
#include <EveryProf.h>

using PPositionAgent = std::unique_ptr<class PositionAgent>;

class PositionAgent : public Agent
{
public:
	AGENT(PositionAgent);

	PAgent Clone(GameObject *parent, const std::string &name = "") override;

	void JsonLoad(const rapidjson::Value &val) override;

	void Update(const GameObjectParams &params) override;

	virtual void DrawGui(float gt) override;

	inline void Set(const WPos &pos) noexcept
	{
		mPos = pos;
	}

	inline const WPos &Get() const noexcept
	{
		return mPos;
	}

	inline WPos &Get() noexcept
	{
		return mPos;
	}

private:
	WPos mPos;

};

#ifndef _a_PositionAgent
#define _a_PositionAgent
REGISTER_AGENT(PositionAgent)
#endif

class Controlable : public Agent
{
public:
	AGENT(Controlable);

	// Унаследовано через Agent
	virtual PAgent Clone(GameObject * parent, const std::string & name = "") override;

	void Update(const GameObjectParams &params) override;

	float GetFreq() const override
	{
		return 1 / 2.0f;
	}
};

REGISTER_AGENT(Controlable)

class Creature : public Agent
{
public:
	AGENT(Creature);

	// Унаследовано через Agent
	virtual PAgent Clone(GameObject * parent, const std::string & name = "") override;

	void DiscardCurrentOrder();

	void look_around(const GameObjectParams & params);

	void make_step(const GameObjectParams & params);

	void Update(const GameObjectParams &params) override;

	void DrawGui(float gt) override;

	void Requirements() override;

	float GetFreq() const override
	{
		return 1 / 10.f;
	}

	void OnCreate(const GameObjectParams & params) override;
	void OnDestroy(const GameObjectParams & params) override;

	void AddPersinal(POrder o);

	POrder order;

	std::deque<POrder> personal;

	std::list<glm::vec3> path;
	glm::vec3 wishpos;
	float step_step = 0;
	glm::vec3 newpos;

	size_t uid = 0;
	static size_t global_uid;
};

REGISTER_AGENT(Creature)

class WalkingPossibility : public Agent
{
public:
	AGENT(WalkingPossibility);

	// Унаследовано через Agent
	virtual PAgent Clone(GameObject * parent, const std::string & name = "") override;

	void Update(const GameObjectParams &params) override;

	void DrawGui(float gt) override;
};

REGISTER_AGENT(WalkingPossibility)

class CrawlingPossibility : public Agent
{
public:
	AGENT(CrawlingPossibility)

		// Унаследовано через Agent
		virtual PAgent Clone(GameObject * parent, const std::string & name = "") override;

	void Update(const GameObjectParams &params) override;

	void DrawGui(float gt) override;
};

REGISTER_AGENT(CrawlingPossibility)

class WaterConsumer : public Agent
{
public:
	AGENT(WaterConsumer);

	// Унаследовано через Agent
	virtual PAgent Clone(GameObject * parent, const std::string & name = "") override;

	void Update(const GameObjectParams &params) override;

	void DrawGui(float gt) override;

	float GetFreq() const override
	{
		return 4.0f;
	}

	float water = 100, full = 100;
};

REGISTER_AGENT(WaterConsumer)

class CalorieConsumer : public Agent
{
public:
	AGENT(CalorieConsumer);

	// Унаследовано через Agent
	virtual PAgent Clone(GameObject * parent, const std::string & name = "") override;

	void Update(const GameObjectParams &params) override;

	void DrawGui(float gt) override;

	float GetFreq() const override
	{
		return 5.0f;
	}

	float calorie = 100, full = 100;
	bool want_to_eat = false;
};

REGISTER_AGENT(CalorieConsumer)

struct Mind
{
	std::string mind;
};

class Anatomic : public Agent
{
public:
	AGENT(Anatomic);

	Anatomic();

	// Унаследовано через Agent
	virtual PAgent Clone(GameObject * parent, const std::string & name = "") override;

	void Update(const GameObjectParams &params) override;

	void Afterload(GameObject * parent) override;

	void DrawGui(float gt) override;

	void Think(const boost::format & s);

	std::unordered_map<StringIntern, glm::vec3> interest_points;
	boost::circular_buffer<Mind> minds;

	void Think(const std::string &s);
	void Interest(const std::string &s, glm::vec3 &p);
};

REGISTER_AGENT(Anatomic)

class Morale : public Agent
{
public:
	AGENT(Morale);

	// Унаследовано через Agent
	virtual PAgent Clone(GameObject * parent, const std::string & name = "") override;

	void Update(const GameObjectParams &params) override;

	void DrawGui(float gt) override;

	float morale = 100;
};

REGISTER_AGENT(Morale)

class Named : public Agent
{
public:
	AGENT(Named);

	// Унаследовано через Agent
	virtual PAgent Clone(GameObject * parent, const std::string & name = "") override;

	void Update(const GameObjectParams &params) override;

	virtual void OnCreate(const GameObjectParams & params) override;

	void DrawGui(float gt) override;

	std::string name;
	char gender = 'f';
};

REGISTER_AGENT(Named)

class DeathDrop : public Agent
{
public:
	AGENT(DeathDrop);

	// Унаследовано через Agent
	virtual PAgent Clone(GameObject * parent, const std::string & name = "") override;

	void Update(const GameObjectParams &params) override;

	virtual void OnDestroy(const GameObjectParams & params) override;

	void JsonLoad(const rapidjson::Value &val) override;

	StringIntern id;
};

REGISTER_AGENT(DeathDrop)

class Wander : public Agent
{
public:
	AGENT(Wander);

	// Унаследовано через Agent
	virtual PAgent Clone(GameObject * parent, const std::string & name = "") override;

	void Update(const GameObjectParams &params) override;

	float GetFreq() const override
	{
		return 10.0f;
	}
};

REGISTER_AGENT(Wander)

class Food : public Agent
{
public:
	AGENT(Food);

	// Унаследовано через Agent
	virtual PAgent Clone(GameObject * parent, const std::string & name = "") override;
	void JsonLoad(const rapidjson::Value &val) override;

	inline int GetNutrition() const
	{
		return nutrition;
	}

	int nutrition;
};

REGISTER_AGENT(Food)

class ActivityConsumer : public Agent
{
public:
	AGENT(ActivityConsumer);

	// Унаследовано через Agent
	virtual PAgent Clone(GameObject * parent, const std::string & name = "") override;

	void Update(const GameObjectParams &params) override;

	void DrawGui(float gt) override;

	void Tire(float t);
	bool IsTired() const;
	bool IsRested() const;

	float GetFreq() const override
	{
		return 6.0f;
	}

	float activity = 100, full = 100;
};

REGISTER_AGENT(ActivityConsumer)

struct Relationships
{
	float value = 0;

	std::string to_string() const;
	std::string with;
};

class Talker : public Agent
{
public:
	AGENT(Talker);

	// Унаследовано через Agent
	virtual PAgent Clone(GameObject * parent, const std::string & name = "") override;

	void Update(const GameObjectParams &params) override;

	void DrawGui(float gt) override;

	float GetFreq() const override
	{
		return 4.5f;
	}

	std::map<size_t, Relationships> relationships;
};

REGISTER_AGENT(Talker)

class ProfessionPerformer : public Agent
{
public:
	AGENT(ProfessionPerformer);

	// Унаследовано через Agent
	virtual PAgent Clone(GameObject * parent, const std::string & name = "") override;

	void Update(const GameObjectParams &params) override;

	void DrawGui(float gt) override;

	float GetFreq() const override
	{
		return 4.5f;
	}

	bool CanPeformOrder(POrder o);

	std::vector<PProfession> prof = {std::make_shared<EveryProf>()};
};

REGISTER_AGENT(ProfessionPerformer)

class ChainDestruction : public Agent
{
public:
	AGENT(ChainDestruction);

	// Унаследовано через Agent
	virtual PAgent Clone(GameObject * parent, const std::string & name = "") override;

	void Update(const GameObjectParams &params) override;

	void DrawGui(float gt) override;

	void OnDestroy(const GameObjectParams &params) override;

	void JsonLoad(const rapidjson::Value &val) override;

	float GetFreq() const override
	{
		return 4.5f;
	}

	std::vector<StringIntern> destroys;
	bool destroyed = false;
};

REGISTER_AGENT(ChainDestruction)

class BasicWorkbench : public Agent
{
public:
	AGENT(BasicWorkbench);

	// Унаследовано через Agent
	virtual PAgent Clone(GameObject * parent, const std::string & name = "") override;

	void DrawGui(float gt) override;

	float GetFreq() const override
	{
		return std::numeric_limits<float>::max();
	}
};

REGISTER_AGENT(BasicWorkbench)