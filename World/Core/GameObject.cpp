



#include "GameObject.h"
#include <boost/serialization/shared_ptr.hpp>


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

void GameObject::DrawGui(float gt)
{
	for (auto &agent : mAgents)
	{
		agent.second->DrawGui(gt);
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

void GameObject::save(boost::archive::binary_oarchive& ar, const unsigned) const
{
	ar << std::string(id);
	for(const auto &a : mAgents)
	{
		ar << a;
	}
}

void GameObject::load(boost::archive::binary_oarchive& ar, const unsigned)
{
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