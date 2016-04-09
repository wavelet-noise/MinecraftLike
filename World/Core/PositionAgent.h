



#pragma once
#ifndef PositionAgent_h__
#define PositionAgent_h__

#include "Agent.h"
#include "..\tools\CoordSystem.h"
#include <Core\OrderBus.h>


using PPositionAgent = std::unique_ptr<class PositionAgent>;

class PositionAgent : public Agent
{
public:
  PositionAgent();
  PositionAgent(GameObject *parent, const std::string &name = "");
  PositionAgent(const PositionAgent &object, GameObject *parent, const std::string &name = "");
  ~PositionAgent();

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
	Controlable();

	// Унаследовано через Agent
	virtual PAgent Clone(GameObject * parent, const std::string & name = "") override;

	void Update(const GameObjectParams &params) override;
};

REGISTER_AGENT(Controlable)

class Creature : public Agent
{
public:
	Creature();

	// Унаследовано через Agent
	virtual PAgent Clone(GameObject * parent, const std::string & name = "") override;

	void Update(const GameObjectParams &params) override;

	void DrawGui() override;
	

	POrder order;
};

REGISTER_AGENT(Creature)

class WalkingPossibility : public Agent
{
public:
	WalkingPossibility();

	// Унаследовано через Agent
	virtual PAgent Clone(GameObject * parent, const std::string & name = "") override;

	void Update(const GameObjectParams &params) override;

	void DrawGui() override;
};

REGISTER_AGENT(WalkingPossibility)

class CrawlingPossibility : public Agent
{
public:
	CrawlingPossibility();

	// Унаследовано через Agent
	virtual PAgent Clone(GameObject * parent, const std::string & name = "") override;

	void Update(const GameObjectParams &params) override;

	void DrawGui() override;
};

REGISTER_AGENT(CrawlingPossibility)

class WaterConsumer : public Agent
{
public:
	WaterConsumer();

	// Унаследовано через Agent
	virtual PAgent Clone(GameObject * parent, const std::string & name = "") override;

	void Update(const GameObjectParams &params) override;

	void DrawGui() override;

	float water = 0, full = 100;
};

REGISTER_AGENT(WaterConsumer)

class CalorieConsumer : public Agent
{
public:
	CalorieConsumer();

	// Унаследовано через Agent
	virtual PAgent Clone(GameObject * parent, const std::string & name = "") override;

	void Update(const GameObjectParams &params) override;

	void DrawGui() override;

	float calorie = 0, full = 100;
};

REGISTER_AGENT(CalorieConsumer)

#endif // PositionAgent_h__