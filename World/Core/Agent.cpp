#include "Agent.h"
#include "GameObject.h"

void Agent::Update(const GameObjectParams & params)
{
}

void Agent::Requirements()
{
}

void Agent::Afterload(GameObject * parent)
{
}

bool Agent::DrawGui(float gt)
{
	return false;
}

void Agent::Interact(const InteractParams & params)
{
}

void Agent::OnDestroy(const GameObjectParams & params)
{
}

void Agent::OnCreate(const GameObjectParams & params)
{
}

void Agent::OnAdjacentChanged(const GameObjectParams & params)
{
}

void Agent::JsonLoad(const rapidjson::Value & val)
{
}


AgentFactory::FactoryType &AgentFactory::Get()
{
  static FactoryType object;
  return object;
}
