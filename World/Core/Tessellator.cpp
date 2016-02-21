// ============================================================================
// ==         Copyright (c) 2016, Samsonov Andrey and Smirnov Denis          ==
// ==                  See license.txt for more information                  ==
// ============================================================================
#include "Tessellator.h"
#include "..\Render\Render.h"
#include "DB.h"
#include <utility>



Tessellator::Tessellator(Render &render)
  : mRender(render)
{
}

void Tessellator::Set(const WBPos &pos, PBlockTessellator block)
{
//   PushFunc([this](const WBPos &pos, PBlockTessellator block)
//   {
//     auto spos = cs::WBtoS(pos);
//     auto sector = FindSector(spos);
//     if (!sector)
//     {
//       sector = std::make_shared<SectorTessellator>(spos);
//       mSectors[spos] = sector;
//     }
//     sector->SetBlock(cs::WBtoSB(pos, spos), block);
//   }, pos, block);
}

void Tessellator::Set(const SPos &spos, std::vector<std::tuple<size_t, StringIntern>> &&blocks)
{
  PushFunc([&, this](const std::tuple<SPos, std::vector<std::tuple<size_t, StringIntern>>> &tuple)
  {
    const auto &spos = std::get<0>(tuple);
    auto &blocks = std::get<1>(tuple);

    auto sector = FindSector(spos);
    if (!sector)
    {
      sector = std::make_shared<SectorTessellator>(spos);
      mSectors[spos] = sector;
    }
    for (size_t i = 0; i < blocks.size(); ++i)
    {
      sector->SetBlock(cs::ItoSB(std::get<0>(blocks[i])), DB::Get().CreateTesselator(std::get<1>(blocks[i])));
    }
  }, spos, std::move(blocks));
}

void Tessellator::SayChanged(const SPos &pos)
{
  PushFunc([this](const std::tuple<SPos> &tuple)
  {
    if (auto sector = FindSector(std::get<0>(tuple)))
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
  std::this_thread::sleep_for(std::chrono::milliseconds(1));
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
