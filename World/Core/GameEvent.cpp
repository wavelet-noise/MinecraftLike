// ============================================================================
// ==         Copyright (c) 2016, Samsonov Andrey and Smirnov Denis          ==
// ==                  See license.txt for more information                  ==
// ============================================================================
#include "GameEvent.h"
#include "World.h"
#include "Chest.h"


void GameEventMove::Execute(World *world)
{
  world->GetPlayer()->Move(mMoved);
}

void GameEventRotate::Execute(World *world)
{
  world->GetPlayer()->Rotate(mRotated);
}

void GameEventSelectPlayerSlot::Execute(World * world)
{
  world->GetPlayer()->GetFromFullName<Chest>("Chest")->Select(mSlot);
}
