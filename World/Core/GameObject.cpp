



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
		agent.second->__Update(params);
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

void GameObject::Requirements()
{
	for (auto &agent : mAgents)
	{
		agent.second->Requirements();
	}
}

void GameObject::Afterload()
{
	for (auto &agent : mAgents)
	{
		agent.second->Afterload(this);
	}
}

void GameObject::OnDestroy(const GameObjectParams &params)
{
	for (auto &agent : mAgents)
	{
		agent.second->OnDestroy(params);
	}
}

void GameObject::OnCreate(const GameObjectParams & params)
{
	for (auto &agent : mAgents)
	{
		agent.second->OnCreate(params);
	}
}

void GameObject::OnAdjacentChanged(const GameObjectParams & params)
{
	for (auto &agent : mAgents)
	{
		agent.second->OnAdjacentChanged(params);
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

void GameObject::PushAgent(PAgent go)
{
	mAgents[go->GetFullName()] = go;
}

std::string GameObject::GetDescription()
{
	std::stringstream ss;

	ss << id;

	return ss.str();
}