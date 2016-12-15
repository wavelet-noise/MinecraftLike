#pragma once
#include <Core/agents/Agent.h>
#include "../GameObject.h"
#include <vector>
#include "../ChestSlot.h"
#include <tools/CoordSystem.h>
#include <list>

class Chest : public Agent
{
public:
	Chest();
	AGENT(Chest);

	void JsonLoad(const rapidjson::Value &val) override;

	// ”наследовано через Agent
	PAgent Clone(GameObject * parent, const std::string & name = "") override;
	void Update(const GameObjectParams & params) override;

	bool Push(PGameObject go, int count = 1, int pos = -1);
	bool CanPush(PGameObject go, int count = 1, int pos = -1) const;
	const ChestSlot GetFirst() const;
	const ChestSlot GetFirst(int & pos) const;
	ChestSlot PopFirst();
	ChestSlot PopSlot(int slot);
	ChestSlot PopFirst(int & pos);
	ChestSlot PopByPredicate(std::function<bool(const ChestSlot &)> pred);
	ChestSlot GetByPredicate(std::function<bool(const ChestSlot&)> pred);

	// client
	// рисует gui этого агента дл€ переданного в параметрах блока. ƒолжен вызыватьс€ каждый кадр, когда требуетс€ отрисовка окна
	bool DrawGui(const GameObjectParams& params, float gt) override;

	int columns = 10;
	int size = 40;
	std::vector<ChestSlot> slots;
	float max_volume = -1, volume = 0;
};

REGISTER_AGENT(Chest)

class InputChest : public Chest
{
public:
	AGENT(InputChest)
		virtual PAgent Clone(GameObject * parent, const std::string & name = "") override;
};

REGISTER_AGENT(InputChest)

class OutputChest : public Chest
{
public:
	AGENT(OutputChest)
		virtual PAgent Clone(GameObject * parent, const std::string & name = "") override;
};

REGISTER_AGENT(OutputChest)