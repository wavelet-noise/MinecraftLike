// ============================================================================
// ==         Copyright (c) 2016, Samsonov Andrey and Smirnov Denis          ==
// ==                  See license.txt for more information                  ==
// ============================================================================
#include "Mesh.h"
#include "Shader.h"
#include "RenderMeshVao.h"
#include "..\Core\MeshPartialBlockGenerator.h"
#include "TemplateMesh.h"
#include <tiny_obj_loader.h>
#include <tools\Log.h>

//buildaabb + presend + send + clear
void Mesh::Compile(Shader &shader)
{
  BuildAABB();
  Presend(shader);
  Send(shader);
  Clear();
}

void Mesh::Presend(Shader &shader) const
{
  mStrategy = std::make_unique<RenderMeshVao>();

  auto locations = shader.GetAttributeLocation(mAttribute);
  mStrategy->SetAttribute(mAttribute, locations);
}

void Mesh::Send(Shader &shader) const
{
  mStrategy->Send(mVertex.data(), mVertex.size(), mIndex.data(), mIndex.size());
}

void Mesh::Clear()
{
  mVertex.clear();
  mVertex.shrink_to_fit();
  mIndex.clear();
  mIndex.shrink_to_fit();
}

void Mesh::Draw() const
{
  mStrategy->Draw();
}

void Mesh::Load(const std::string & s)
{
  std::vector<tinyobj::shape_t> shapes;
  std::vector<tinyobj::material_t> mats;
  std::string err;

  bool ok = tinyobj::LoadObj(shapes, mats, err, s.c_str());
  if (!ok)
  {
    LOG(error) << err;
  }

  mVertex.reserve((shapes[0].mesh.positions.size() / 3) * (3 + 2 + 3));
  for (size_t i = 0; i < shapes[0].mesh.positions.size() / 3; ++i)
  {
    mVertex.push_back(shapes[0].mesh.positions[i * 3]); mVertex.push_back(shapes[0].mesh.positions[i * 3 + 1]); mVertex.push_back(shapes[0].mesh.positions[i * 3 + 2]);
    mVertex.push_back(shapes[0].mesh.texcoords[i * 2]); mVertex.push_back(shapes[0].mesh.texcoords[i * 2 + 1]);
    mVertex.push_back(shapes[0].mesh.normals[i * 3]); mVertex.push_back(shapes[0].mesh.normals[i * 3 + 1]); mVertex.push_back(shapes[0].mesh.normals[i * 3 + 2]);
  }
  mIndex = shapes[0].mesh.indices;

  mAttribute = VertexVTN::Get();
}

void Mesh::BuildAABB()
{
  BuildAABB(mAttribute[0]);
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
