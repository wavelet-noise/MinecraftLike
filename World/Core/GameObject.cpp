// ============================================================================
// ==                   Copyright (c) 2015, Smirnov Denis                    ==
// ==                  See license.txt for more information                  ==
// ============================================================================
#include "GameObject.h"



GameObject::GameObject(const StringIntern &__id) :id(__id)
{
}


GameObject::~GameObject()
{
}

void GameObject::Update(GameObjectParams &params)
{
  for (auto &agent : mAgents)
  {
    agent.second->Update(params);
  }
}

StringIntern GameObject::GetId()
{
  return id;
}

Agent *GameObject::GetFromFullName(const StringIntern &name)
{
  auto it = mAgents.find(name);
  if (it != mAgents.end())
  {
    return it->second.get();
  }

  return nullptr;
}

const Agent *GameObject::GetFromFullName(const StringIntern &name) const
{
  auto it = mAgents.find(name);
  if (it != mAgents.end())
  {
    return it->second.get();
  }

  return nullptr;
}