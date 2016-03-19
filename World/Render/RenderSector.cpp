



#include "RenderSector.h"
#include "Camera.h"
#include <glm\gtc\matrix_transform.hpp>
#include "..\tools\CoordSystem.h"
#include <GLFW\glfw3.h>
#include "Resourses.h"


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
  camera.Update();
  AddElements();

  for (auto &i : mModels)
  {
    auto &model = i.second;
    glm::mat4 matrix = glm::translate(glm::mat4(1), cs::StoW(i.first));

    const auto &aabb = model.GetAABB();
    if (model.IsAabbDot() || !camera.BoxWithinFrustum(matrix * std::get<0>(aabb), matrix * std::get<1>(aabb)))
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
    shader->SetUniform(light.GetViewProject() * matrix, "shadow_VP");
	shader->SetUniform(light.GetPos() - camera.GetPos(), "lightpos");

    model.GetMesh()->Draw();
    mDc++;
  }
}

void RenderSector::ShadowDraw(Camera &camera, PShader shader)
{
  camera.Update();
  AddElements();
  mDc = 0;

  for (auto &i : mModels)
  {
    auto &model = i.second;
    glm::mat4 matrix = glm::translate(glm::mat4(1), cs::StoW(i.first));

    const auto &aabb = model.GetAABB();
    if (model.IsAabbDot() || !camera.BoxWithinFrustum(matrix * std::get<0>(aabb), matrix * std::get<1>(aabb)))
    {
      continue;
    }

    shader->Use();

    shader->SetUniform(camera.GetViewProject() * matrix, "transform_VP");

    model.GetMesh()->Draw();
    mDc++;
  }
}

int RenderSector::ApproxDc()
{
  return mDc;
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

    model.GetMesh()->Compile(model.GetShader()->GetAttributeLocation(model.GetMesh()->GetAttribute()));

    model.GetMesh()->Release();

    mModels[std::get<1>(i)] = std::move(model);
  }
}
