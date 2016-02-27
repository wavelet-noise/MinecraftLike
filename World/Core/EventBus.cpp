// ============================================================================
// ==         Copyright (c) 2016, Samsonov Andrey and Smirnov Denis          ==
// ==                  See license.txt for more information                  ==
// ============================================================================
#include "EventBus.h"



void EventBus::Update(class World *world)
{
  if (mEvents.empty())
  {
    return;
  }

  for (auto &e : mEvents)
  {
    e->Execute(world);
  }

  mEvents.clear();
}
