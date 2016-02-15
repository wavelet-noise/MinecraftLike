// ============================================================================
// ==         Copyright (c) 2016, Samsonov Andrey and Smirnov Denis          ==
// ==                  See license.txt for more information                  ==
// ============================================================================
#include "RenderList.h"
#include "Render.h"
#include <assert.h>
#include "Camera.h"



RenderList::RenderList(Render &render)
  : mRender(render)
{
}


RenderList::~RenderList()
{
}

RenderIterator RenderList::PushModel(const Model &model, const glm::mat4 &matrix)
{
  std::lock_guard<std::mutex> lock(mMutex);

  mAddList.push_back({ model, matrix });
  mAddList.back().mIterator = std::make_shared<Element::Iterator>(--mAddList.end());

  return{ mAddList.back().mIterator, mMutex };
}

void RenderList::Draw(Camera &camera)
{
  AddElements();
  SwapMatrix();

  for (auto &i : mDrawList)
  {
    i.model.GetTexture()->Set(TEXTURE_SLOT_0);

    auto &shader = i.model.GetShader();
    shader->Use();
    shader->SetUniform(TEXTURE_SLOT_0, "atlas");

    //TODO: prebuild NVP
    shader->SetUniform(camera.GetProject() * camera.GetView() * i.matrix, "transform_VP");

    i.model.GetMesh()->Draw();
  }
}

void RenderList::AddElements()
{
  std::lock_guard<std::mutex> lock(mMutex);
  if (mAddList.empty())
  {
    return;
  }

  for (auto &i : mAddList)
  {
    mDrawList.push_back(i);
    auto &element = mDrawList.back();
    *element.mIterator = --mDrawList.end();

    element.model.GetMesh()->Compile(element.model.GetShader()->GetAttributeLocation(element.model.GetMesh()->GetAttribute()));
    element.model.GetMesh()->Release();
  }
  mAddList.clear();
}

void RenderList::SwapMatrix()
{
  std::lock_guard<std::mutex> lock(mMutex);
  for (auto &i : mDrawList)
  {
    //std::swap(i.matrix, i.matrixBack);
    i.matrix = i.matrixBack;
  }
}
