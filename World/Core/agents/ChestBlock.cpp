#include "ChestBlock.h"
#include <Game.h>
#include <Core/MapGen/WorldWorker.h>
#include <gui/WindowStorages.h>

PAgent ColonyStorage::Clone(GameObject * parent, const std::string &name)
{
	auto t = MakeAgent<ColonyStorage>(*this);
	t->mParent = parent;
	return t;
}

void ColonyStorage::OnCreate(const GameObjectParams& params)
{
	Storages::Get().AddStorage(params.pos, mParent->shared_from_this());
}

void ColonyStorage::OnDestroy(const GameObjectParams& params)
{
	Storages::Get().RemoveStorage(mParent->shared_from_this());
}
