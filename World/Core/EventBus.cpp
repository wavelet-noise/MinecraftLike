



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
