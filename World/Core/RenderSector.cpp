// ============================================================================
// ==         Copyright (c) 2016, Samsonov Andrey and Smirnov Denis          ==
// ==                  See license.txt for more information                  ==
// ============================================================================
#include "RenderSector.h"
#include "Sector.h"
#include <GLFW\glfw3.h>
#include <type_traits>
#include "..\tools\Log.h"
#include <glm\gtc\matrix_transform.hpp>



RenderSector::RenderSector(const SPos &pos)
{
  using MeshType = std::remove_reference_t<decltype(mModel.GetMesh())>::element_type;
  mModel.GetMesh() = std::make_shared<MeshType>();
  mModel.GetMesh()->Reserve(100000, 100000);

  mModelMatrix = glm::translate(mModelMatrix, cs::StoW(pos));
}



RenderSector::~RenderSector()
{
}

void RenderSector::SayChanged()
{
  mIsChanged = true;
}

bool RenderSector::IsNeedBuild() const
{
  return mIsNeedBuild;
}

void RenderSector::Push(const Model &model, const WPos &pos)
{
  // Если сектор надо перестроить и модель статическая - перестраиваем сектор.
  if (mIsNeedBuild && model.GetType() == Model::Static)
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
}

void RenderSector::Update(Render &render)
{
  // Сектор должен был перестроиться. К этому моменту он уже перестроен.
  if (mIsNeedBuild)
  {
    render.PushModel(mModel, mModelMatrix);
    mIsNeedBuild = false;
  }

  // Геометрия изменилась, нужно его перестроить.
  if (mIsChanged)
  {
    mIsNeedBuild = true;
    mIsChanged = false;
  }
}
