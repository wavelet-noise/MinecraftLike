#pragma once
#include <memory>

class GameObject;

class ChestSlot
{
public:
  std::shared_ptr<GameObject> obj;
  float count;

  void DrawGui(bool selected = false);
};