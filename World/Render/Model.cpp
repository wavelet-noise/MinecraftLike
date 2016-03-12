// ============================================================================
// ==         Copyright (c) 2016, Samsonov Andrey and Smirnov Denis          ==
// ==                  See license.txt for more information                  ==
// ============================================================================

#include "Model.h"
#include "Resourses.h"
#include <Serealize.h>
#include"TextureManager.h"

Model::Model()
{
  mShader = Resourses::Get().GetShader("shaders/basic.glsl");
}

void Model::JsonLoad(const rapidjson::Value & val)
{
  if (val.HasMember("sprite"))
  {
    auto s = val["sprite"].GetString();

    mTexture = std::get<0>(TextureManager::Get().GetTexture(s));
    mSprite = s;
  }
}

void Model::SetSprite(const StringIntern & s)
{
  mTexture = std::get<0>(TextureManager::Get().GetTexture(s));
  mSprite = s;
}

void Model::BuildAABB(glm::vec3 VertexVTN::* p)
{
  glm::vec3 _min{ 0 }, _max{ 0 };
  if (!mMesh->Empty())
  {
    for (decltype(mMesh->SizeVertex()) i = 0; i < mMesh->SizeVertex(); ++i)
    {
      const glm::vec3 &t = mMesh->Vertex(i).*p;
      for (int j = 0; j < 3; ++j)
      {
        _max[j] = glm::max(t[j], _max[j]);
        _min[j] = glm::min(t[j], _min[j]);
      }
    }
    empty_aabb = false;
  }
  else
  {
    empty_aabb = true;
  }
  min = glm::vec4(_min, 1);
  max = glm::vec4(_max, 1);
}

bool Model::IsAabbDot()
{
  return empty_aabb;
}
