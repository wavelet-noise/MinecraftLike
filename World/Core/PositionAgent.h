#pragma once


#include "Agent.h"
#include "..\tools\CoordSystem.h"
#include <Core\OrderBus.h>
#include <unordered_map>
#include <boost\circular_buffer.hpp>

using PPositionAgent = std::unique_ptr<class PositionAgent>;

class PositionAgent : public Agent
{
public:
	AGENT(PositionAgent)

		PAgent Clone(GameObject *parent, const std::string &name = "") override;

	void JsonLoad(const rapidjson::Value &val) override;

	void Update(const GameObjectParams &params) override;

	virtual void DrawGui() override;

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

REGISTER_AGENT(PositionAgent)

class Controlable : public Agent
{
public:
	AGENT(Controlable)

		// Унаследовано через Agent
		virtual PAgent Clone(GameObject * parent, const std::string & name = "") override;

	void Update(const GameObjectParams &params) override;

	float GetFreq() const override
	{
		return 1.0f;
	}
};

REGISTER_AGENT(Controlable)

class Creature : public Agent
{
public:
	AGENT(Creature)

		// Унаследовано через Agent
		virtual PAgent Clone(GameObject * parent, const std::string & name = "") override;

	void Update(const GameObjectParams &params) override;

	void DrawGui() override;

	float GetFreq() const override
	{
		return 1 / 20.f;
	}

	POrder order;
};

REGISTER_AGENT(Creature)

class WalkingPossibility : public Agent
{
public:
	AGENT(WalkingPossibility)

		// Унаследовано через Agent
		virtual PAgent Clone(GameObject * parent, const std::string & name = "") override;

	void Update(const GameObjectParams &params) override;

	void DrawGui() override;
};

REGISTER_AGENT(WalkingPossibility)

class CrawlingPossibility : public Agent
{
public:
	AGENT(CrawlingPossibility)

		// Унаследовано через Agent
		virtual PAgent Clone(GameObject * parent, const std::string & name = "") override;

	void Update(const GameObjectParams &params) override;

	void DrawGui() override;
};

REGISTER_AGENT(CrawlingPossibility)

class WaterConsumer : public Agent
{
public:
	AGENT(WaterConsumer)

		// Унаследовано через Agent
		virtual PAgent Clone(GameObject * parent, const std::string & name = "") override;

	void Update(const GameObjectParams &params) override;

	void DrawGui() override;

	float GetFreq() const override
	{
		return 5.0f;
	}

	float water = 0, full = 100;
};

REGISTER_AGENT(WaterConsumer)

class CalorieConsumer : public Agent
{
public:
	AGENT(CalorieConsumer)

		// Унаследовано через Agent
		virtual PAgent Clone(GameObject * parent, const std::string & name = "") override;

	void Update(const GameObjectParams &params) override;

	void DrawGui() override;

	float GetFreq() const override
	{
		return 5.0f;
	}

	float calorie = 0, full = 100;
};

REGISTER_AGENT(CalorieConsumer)

struct Mind
{
	std::string mind;
};

class Anatomic : public Agent
{
public:
	AGENT(Anatomic)

		Anatomic();

	// Унаследовано через Agent
	virtual PAgent Clone(GameObject * parent, const std::string & name = "") override;

	void Update(const GameObjectParams &params) override;

	void Afterload(GameObject * parent) override;

	void DrawGui() override;

	std::unordered_map<StringIntern, glm::vec3> interest_points;
	boost::circular_buffer<Mind> minds;

	void Think(const std::string &s);
};

REGISTER_AGENT(Anatomic)

class Morale : public Agent
{
public:
	AGENT(Morale)

		// Унаследовано через Agent
		virtual PAgent Clone(GameObject * parent, const std::string & name = "") override;

	void Update(const GameObjectParams &params) override;

	void DrawGui() override;

	float morale;
};

REGISTER_AGENT(Morale)

class Named : public Agent
{
public:
	AGENT(Named)

		// Унаследовано через Agent
		virtual PAgent Clone(GameObject * parent, const std::string & name = "") override;

	void Update(const GameObjectParams &params) override;

	virtual void OnCreate(const GameObjectParams & params) override;

	void DrawGui() override;

	std::string name;
	char gender = 'f';
};

REGISTER_AGENT(Named)

class DeathDrop : public Agent
{
public:
	AGENT(DeathDrop)

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
	AGENT(Wander)

		// Унаследовано через Agent
		virtual PAgent Clone(GameObject * parent, const std::string & name = "") override;

	void Update(const GameObjectParams &params) override;

	float GetFreq() const override
	{
		return 1.0f;
	}
};

REGISTER_AGENT(Wander)