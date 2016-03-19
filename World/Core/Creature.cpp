



#include "Creature.h"
#include "PhysicAgent.h"


const StringIntern Creature::mPositionAgentName = StringIntern("PositionAgent");


Creature::Creature(const StringIntern &__id) : GameObject(__id)
{
  auto positionAgent = std::make_unique<PositionAgent>(this);
  mAgents[positionAgent->GetFullName()] = std::move(positionAgent);
  auto physicAgent = std::make_unique<PhysicAgent>(this);
  mAgents[physicAgent->GetFullName()] = std::move(physicAgent);
}


Creature::~Creature()
{
}

PGameObject Creature::Clone()
{
  return MakeGameObject<Creature>(id);
}

void Creature::Update(GameObjectParams &params)
{
  params.pos = GetFromFullName<PositionAgent>(mPositionAgentName)->Get();
  GameObject::Update(params);
}
