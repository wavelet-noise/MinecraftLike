



#pragma once
#ifndef EventBus_h__
#define EventBus_h__

#include <list>
#include <memory>
#include "GameEvent.h"


class EventBus
{
public:

  void Update(class World *world);

  void Push(std::unique_ptr<GameEvent> event)
  {
    mEvents.push_back(std::move(event));
  }

private:
  std::list<std::unique_ptr<GameEvent>> mEvents;

};



#endif // EventBus_h__
