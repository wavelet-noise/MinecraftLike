#include "Sector.h"
#include <core\PositionAgent.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <stdint.h>
#include "../tools/Log.h"
#include "../Render/Render.h"
#include "World.h"
#include "Tessellator.h"
#include "SectorBase.h"
#include <tuple>
#include <Render\ParticleSystem.h>


Sector::Sector(const SPos &position)
	: mPos(position)
{
}


Sector::~Sector()
{
}

const SPos & Sector::GetPos() const
{
	return mPos;
}

void Sector::SetBlock(const SBPos &pos, PGameObject block)
{
	SectorBase<PGameObject>::SetBlock(pos, block);

	if (mTessellator)
	{
		if (block)
		{
			mTessellator->Set(cs::SBtoWB(pos, mPos), DB::Get().CreateTesselator(block->GetId()));
			mTessellator->SayChanged(mPos);
		}
		else
		{
			mTessellator->Set(cs::SBtoWB(pos, mPos), nullptr);
			mTessellator->SayChanged(mPos);
		}
	}
}

void Sector::Spawn(const SBPos & position, PGameObject creature)
{
	creatures.push_back(creature);
	EventBus::Get().Publish<EventCreatureSpawn>(creature);

	if (auto p = creature->GetAgent<PositionAgent>())
		p->Set(position);
	else
		LOG(error) << "Spawning " << creature->GetId() << ", that is not creature!";
}

void Sector::Update(World *world, float dt)
{
	GameObjectParams gop{ world, this, {}, dt };
	for (size_t i = 0; i < mUniqueBlocks.size(); ++i)
	{
		gop.pos = cs::SBtoWB(cs::ItoSB(mUniquePoses[i]), mPos);
		if (mUniqueBlocks[i]) mUniqueBlocks[i]->Update(gop);
	}


	for (auto c_iter = creatures.begin(); c_iter != creatures.end(); ++c_iter)
	{
		(*c_iter)->Update(gop);
	}

	for (auto c_iter = creatures.begin(); c_iter != creatures.end();)
	{
		PGameObject c = *c_iter;
		auto p = c->GetAgent<PositionAgent>();

		ParticleSystem::Get().Add( p->Get() + glm::vec3(0,0,0.5), StringIntern("car"), dt);

		// если находится за границей сектора -- переносим в новый сектор, если он существует
		if (p->Get().x > (mPos.x + 1) * SECTOR_SIZE ||
			p->Get().x < (mPos.x) * SECTOR_SIZE ||
			p->Get().y >(mPos.y + 1) * SECTOR_SIZE ||
			p->Get().y < (mPos.y) * SECTOR_SIZE)
		{
			auto &nsec = world->GetSector(cs::WtoS(p->Get()));
			if (nsec)
			{
				creatures.erase(c_iter);
				nsec->creatures.push_back(std::move(c));
				break;
			}
		}
		//передвигаем итератор, если существо не перемещено
		++c_iter;
	}
}



void Sector::Draw(class Tessellator *tess)
{
	mTessellator = tess;
	if (mTessellator /*&& !mBlocks.empty()*/)
	{
		auto currentTime = glfwGetTime();
		std::vector<std::tuple<size_t, StringIntern>> blocks;
		blocks.reserve(mCountBlocks);
		for (size_t i = 0; i < mBlocks.size(); ++i)
		{
			if (mBlocks[i])
			{
				blocks.emplace_back(i, GetBlock(i)->GetId());
			}
		}
		mTessellator->Set(mPos, std::move(blocks));
		mTessellator->SayChanged(mPos);
		//LOG(trace) << "SectorOnDraw: " << glfwGetTime() - currentTime;
	}
}
