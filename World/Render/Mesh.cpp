// ============================================================================
// ==         Copyright (c) 2016, Samsonov Andrey and Smirnov Denis          ==
// ==                  See license.txt for more information                  ==
// ============================================================================
#include "Mesh.h"
#include "Shader.h"
#include "RenderMeshVao.h"

void Mesh::Compile(Shader &shader)
{
  BuildAABB(mAttribute[0]);

  mStrategy = std::make_unique<RenderMeshVao>();

  auto locations = shader.GetAttributeLocation(mAttribute);
  mStrategy->SetAttribute(mAttribute, locations);

  mStrategy->Compile(mVertex.data(), mVertex.size(), mIndex.data(), mIndex.size());

  mVertex.clear();
  mVertex.shrink_to_fit();
  mIndex.clear();
  mIndex.shrink_to_fit();
}

void Mesh::Draw()
{
  mStrategy->Draw();
}

void Mesh::BuildAABB(const Attribute &attributeVertex)
{
  // TODO: Переделать цикл по вершинам, вместо цикла по индексам.
  glm::vec3 _min, _max;
  for (size_t i = 0; i < mIndex.size(); ++i)
  {
    const glm::vec3 &t = *reinterpret_cast<glm::vec3 *>(
      &mVertex[(attributeVertex.size * mIndex[i] + attributeVertex.offset) / sizeof(float)]);

    for (int j = 0; j < 3; ++j)
    {
      _max[j] = glm::max(t[j], _max[j]);
      _min[j] = glm::min(t[j], _min[j]);
    }
  }

  std::get<MinAABB>(mAABB) = glm::vec4(_min, 1);
  std::get<MaxAABB>(mAABB) = glm::vec4(_max, 1);
}
