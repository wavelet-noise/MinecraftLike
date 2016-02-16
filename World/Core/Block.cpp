#include "Block.h"
#include <memory>
#include "BlockTessellator.h"

const StringIntern Block::mRenderAgentName = StringIntern("RenderAgent");

Block::Block(const StringIntern &__id) : GameObject(__id)
{
}
 

Block::~Block()
{
}

PGameObject Block::Clone()
{
  return MakeGameObject<Block>(id);
}

void Block::Update(GameObjectParams &params)
{
  for (auto &agent : mAgents)
  {
    if (agent.first != mRenderAgentName)
    {
      agent.second->Update(params);
    }
  }
}

void Block::Draw(GameObjectParams &params)
{
  for (auto &agent : mAgents)
  {
    if (agent.first == mRenderAgentName)
    {
      agent.second->Update(params);
    }
  }
}