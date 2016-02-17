// ============================================================================
// ==         Copyright (c) 2016, Samsonov Andrey and Smirnov Denis          ==
// ==                  See license.txt for more information                  ==
// ============================================================================
#include "Tessellator.h"
#include "..\Render\Render.h"



Tessellator::Tessellator(Render &render)
  : mRender(render)
{
}

void Tessellator::Set(const WBPos &pos, PBlockTessellator block)
{
  PushFunc([this](const WBPos &pos, PBlockTessellator block)
  {
    auto spos = cs::WBtoSB(pos);
    if (auto sector = FindSector(spos))
    {
      sector->SetBlock(cs::WBtoSB(pos, spos), block);
    }
  }, pos, block);
}

void Tessellator::SayChanged(const SPos &pos)
{
  PushFunc([this](const SPos &pos)
  {
    if (auto sector = FindSector(pos))
    {
      sector->SayChanged();
    }
  }, pos);
}

void Tessellator::Start()
{

}

void Tessellator::Process()
{
  for (auto it = mSectors.begin(); it != mSectors.end(); ++it)
  {
    it->second->Update(this, mRender);
  }
}

void Tessellator::Stop()
{

}

PBlockTessellator Tessellator::GetBlock(const WBPos &pos)
{
  auto spos = cs::WBtoS(pos);
  if (auto sector = FindSector(spos))
  {
    return sector->GetBlock(cs::WBtoSB(pos, spos));
  }

  return nullptr;
}

std::shared_ptr<SectorTessellator> Tessellator::FindSector(const SPos &pos)
{
  auto it = mSectors.find(pos);
  if (it != mSectors.end())
  {
    return it->second;
  }

  return nullptr;
}
