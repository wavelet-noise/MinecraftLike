// ============================================================================
// ==         Copyright (c) 2016, Samsonov Andrey and Smirnov Denis          ==
// ==                  See license.txt for more information                  ==
// ============================================================================
#include "RenderSector.h"
#include "Camera.h"
#include <glm\gtc\matrix_transform.hpp>
#include "..\tools\CoordSystem.h"
#include <GLFW\glfw3.h>



void RenderSector::Push(const Model &model, const SPos &pos)
{
  std::lock_guard<std::mutex> lock(mMutex);
  mPushList.emplace_back(model, pos);
}

void RenderSector::Remove(const SPos &pos)
{

}

void RenderSector::Draw(Camera &camera, Camera &light)
{
  AddElements();

  for (auto &i : mModels)
  {
    auto &model = i.second;
    glm::mat4 matrix = glm::translate({}, cs::StoW(i.first));

    const auto &aabb = model.GetAABB();
    if (!camera.BoxWithinFrustum(matrix * std::get<0>(aabb), matrix * std::get<1>(aabb)))
    {
      continue;
    }

    if (auto &texture = model.GetTexture())
    {
      texture->Set(TEXTURE_SLOT_0);
    }
      
    auto &shader = model.GetShader();
    shader->Use();

    shader->SetUniform(camera.GetViewProject() * matrix, "transform_VP");
    shader->SetUniform(light.GetShadow() * matrix, "shadow_VP");
    shader->SetUniform(1, "shadowmap");

    model.GetMesh()->Draw();
  }
}

void RenderSector::ShadowDraw(Camera &camera, PShader shader)
{
  AddElements();

  for (auto &i : mModels)
  {
    auto &model = i.second;
    glm::mat4 matrix = glm::translate({}, cs::StoW(i.first));

    const auto &aabb = model.GetAABB();
    if (!camera.BoxWithinFrustum(matrix * std::get<0>(aabb), matrix * std::get<1>(aabb)))
    {
      continue;
    }

    shader->Use();

    shader->SetUniform(camera.GetViewProject() * matrix, "transform_VP");

    model.GetMesh()->Draw();
  }
}

void RenderSector::AddElements()
{
  decltype(mPushList) tmpList;
  {
    std::lock_guard<std::mutex> lock(mMutex);
    tmpList.swap(mPushList);
  }
  for (auto &i : tmpList)
  {
    auto &model = std::get<0>(i);
    model.BuildAABB(&VertexVTN::vertex);

    auto currentTime = glfwGetTime();
    model.GetMesh()->Compile(model.GetShader()->GetAttributeLocation(model.GetMesh()->GetAttribute()));
    //LOG(trace) << "SectorCompiled: " << glfwGetTime() - currentTime;

    model.GetMesh()->Release();

    mModels[std::get<1>(i)] = std::move(model);
  }
}
