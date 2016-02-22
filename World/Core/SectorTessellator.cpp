// ============================================================================
// ==         Copyright (c) 2016, Samsonov Andrey and Smirnov Denis          ==
// ==                  See license.txt for more information                  ==
// ============================================================================
#include "SectorTessellator.h"

#include <GLFW\glfw3.h>
#include <glm\gtc\matrix_transform.hpp>
#include <type_traits>
#include "Tessellator.h"
#include "TessellatorParams.h"
#include "..\Render\RenderSector.h"
#include "..\tools\Log.h"

SectorTessellator::SectorTessellator(const SPos &pos)
  : mPos(pos)
{
  using MeshType = std::remove_reference_t<decltype(mModel.GetMesh())>::element_type;
  mModel.GetMesh() = std::make_shared<MeshType>();
}

void SectorTessellator::SayChanged()
{
  mChanged = true;
}

void SectorTessellator::Update(Tessellator *tesselator, RenderSector &render)
{
  if (!mChanged)
  {
    return;
  }

  mModel.GetMesh()->Reserve(100000, 100000);

  auto currentTime = glfwGetTime();
  TessellatorParams params{ tesselator, this, mPos,{} };

  for (size_t i = 0; i < mBlocks.size(); ++i)
  {
    if (mBlocks[i])
    {
      auto sbpos = cs::ItoSB(i);
      params.wbpos = cs::SBtoWB(sbpos, mPos);
      Push(GetBlock(i)->GetModel(params), sbpos);
    }
  };

  //LOG(trace) << "SectorTessellated: " << glfwGetTime() - currentTime;
  //LOG(trace) << "SectorTessellated: [" << mPos.x << "," << mPos.y << "," << mPos.z << "]";

  render.Push(mModel, mPos);
  mChanged = false;
}

void SectorTessellator::Push(const Model &model, const SBPos &pos)
{
  auto &dst = mModel.GetMesh();
  const auto &src = model.GetMesh();
  if (dst->Empty())
  {
    mModel.SetTexture(model.GetTexture());
  }
  if (mModel.GetTexture() == model.GetTexture())
  {
    size_t size = dst->SizeVertex();
    dst->Push(*src);
    for (size_t i = size; i < dst->SizeVertex(); ++i)
    {
      dst->Vertex(i).vertex += pos;
    }
  }
  else
  {
    LOG(warning) << "Батчинг меша в секторе пропущен. Текстуры не совпадают.";
  }
}
