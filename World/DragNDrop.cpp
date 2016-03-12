#include "DragNDrop.h"

void DragNDrop::Drag(ChestSlot go)
{
  dragged.obj = go.obj;
  dragged.count = go.count;
}

ChestSlot DragNDrop::Drop()
{
  return std::move(dragged);
}

DragNDrop & DragNDrop::Get()
{
  static DragNDrop dnd;
  return dnd;
}

bool DragNDrop::Busy()
{
  return dragged.obj.get();
}
