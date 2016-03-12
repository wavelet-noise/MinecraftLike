#pragma once
#include <memory>
class ChestSlot
{
public:
  std::shared_ptr<class GameObject> obj;
  int count;
};