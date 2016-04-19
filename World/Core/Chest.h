#pragma once
#include "Agent.h"
#include "GameObject.h"
#include <vector>
#include "ChestSlot.h"

class Chest : public Agent
{
public:
  Chest();
  AGENT(Chest)

  void JsonLoad(const rapidjson::Value &val) override;

  // ”наследовано через Agent
  virtual PAgent Clone(GameObject * parent, const std::string & name = "") override;
  virtual void Update(const GameObjectParams & params) override;
  bool Push(PGameObject go, int count = 1, int pos = -1);
  const ChestSlot GetFirst() const;
  const ChestSlot GetFirst(int & pos) const;
  ChestSlot PopFirst();
  ChestSlot PopSlot(int slot);
  ChestSlot PopFirst(int & pos);
  ChestSlot PopSelected();
  ChestSlot PopByPredicate(std::function<bool(const ChestSlot &)> pred);
  ChestSlot GetByPredicate(std::function<bool(const ChestSlot&)> pred);
  void PushSelected(ChestSlot cs);
  void Select(int slot);
  int GetSelected();


  // client
  // рисует gui этого агента дл€ переданного в параметрах блока. ƒолжен вызыватьс€ каждый кадр, когда требуетс€ отрисовка окна
  virtual void DrawGui();

  int columns = 10;
  int size = 40;
  std::vector<ChestSlot> slots;
  int mSelected = -1;
};

REGISTER_AGENT(Chest)