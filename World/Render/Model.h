// ============================================================================
// ==                   Copyright (c) 2015, Smirnov Denis                    ==
// ==                  See license.txt for more information                  ==
// ============================================================================
#pragma once
#ifndef Model_h__
#define Model_h__

#include "Texture.h"
#include "TemplateMesh.h"
#include "Vertex.h"
#include <rapidjson\document.h>
#include <tools\StringIntern.h>

using PModel = std::shared_ptr<class Model>;

/// Модель. Имеет всю информацию для рисования.
class Model
{
public:
  Model();

  void JsonLoad(const rapidjson::Value & val);

  enum Type
  {
    Static,
  };

  inline void SetTexture(PTexture texture)
  {
    mTexture = texture;
  }

  inline PTemplateMesh<VertexVTN> &GetMesh() noexcept
  {
    return mMesh;
  }

  inline const PTemplateMesh<VertexVTN> &GetMesh() const noexcept
  {
    return mMesh;
  }

  inline const PTexture &GetTexture() const noexcept
  {
    return mTexture;
  }

  inline Type GetType() const noexcept
  {
    return mType;
  }

  inline PShader &GetShader()
  {
    return mShader;
  }

  inline StringIntern GetSpriteName()
  {
    return mSprite;
  }

  void SetSprite(const StringIntern &s);

  void BuildAABB(glm::vec3 VertexVTN::* p);

  const std::tuple<const glm::vec4 &, const glm::vec4 &> GetAABB() const
  {
    return std::tie(min, max);
  }

  bool IsAabbDot();

private:
  glm::vec4 min, max;
  bool empty_aabb = true;
  PTemplateMesh<VertexVTN> mMesh;
  PTexture mTexture;
  PShader mShader;

  StringIntern mSprite = StringIntern(""); //используется для восстановлении связи с атласом

  Type mType = Static;
};

#endif // Model_h__

