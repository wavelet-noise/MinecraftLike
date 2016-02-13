// ============================================================================
// ==         Copyright (c) 2016, Samsonov Andrey and Smirnov Denis          ==
// ==                  See license.txt for more information                  ==
// ============================================================================
#include "RenderList.h"



RenderList::RenderList()
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

void RenderList::Draw()
{
  AddElements();
  SwapMatrix();

  for (auto &i : mDrawList)
  {
    //i.model
    //i.matrix
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
    *mDrawList.back().mIterator = --mDrawList.end();
  }
  mAddList.clear();
}

void RenderList::SwapMatrix()
{
  std::lock_guard<std::mutex> lock(mMutex);
  for (auto &i : mDrawList)
  {
    std::swap(i.matrix, i.matrixBack);
  }
}
