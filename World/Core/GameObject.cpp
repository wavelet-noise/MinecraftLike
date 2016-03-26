



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

void GameObject::DrawGui()
{
  for (auto &agent : mAgents)
  {
    agent.second->DrawGui();
  }
}

void GameObject::Interact(InteractParams & params)
{
  for (auto &agent : mAgents)
  {
    agent.second->Interact(params);
  }
}

void GameObject::Afterload()
{
  for (auto &agent : mAgents)
  {
    agent.second->Afterload(this);
  }
}

PGameObject GameObject::Clone()
{
  auto a = MakeGameObject<GameObject>(id);
  a->placable = placable;

  for (const auto &ag : mAgents)
  {
    a->mAgents[ag.first] = ag.second->Clone(a.get());
  }

  return a;
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

void GameObject::PushAgent(PAgent go)
{
  mAgents[go->GetFullName()] = go;
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

std::string GameObject::GetDescription()
{
	std::stringstream ss;

	ss << id;

	return ss.str();
}