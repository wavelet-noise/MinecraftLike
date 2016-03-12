#pragma once
#include <Core\GameObject.h>
#include <Core\ChestSlot.h>
class DragNDrop
{
public:

  void Drag(ChestSlot go);
  ChestSlot Drop();
  static DragNDrop& Get();
  bool Busy();

private:
  //TODO: сделать выбрасывание предмета на землю, если дроп не произошел ни в один из инвентарей
  ChestSlot dragged;
};