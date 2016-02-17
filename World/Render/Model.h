// ============================================================================
// ==                   Copyright (c) 2015, Smirnov Denis                    ==
// ==                  See license.txt for more information                  ==
// ============================================================================
#pragma once
#ifndef Model_h__
#define Model_h__

#include "Texture.h"
#include "Mesh.h"
#include "Vertex.h"

class Document;

/// Модель. Имеет всю информацию для рисования.
class Model
{
public:
  using VertexType = VertexVTN;

  Model();

  enum Type
  {
    Static,
  };

  inline void SetTexture(PTexture texture)
  {
    mTexture = texture;
  }

  inline PMesh<VertexType> &GetMesh() noexcept
  {
    return mMesh;
  }

  inline const PMesh<VertexType> &GetMesh() const noexcept
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

  PShader &GetShader()
  {
    return mShader;
  }

  void JsonLoad(const rapidjson::Document &val);

private:
  PMesh<VertexType> mMesh;
  PTexture mTexture;
  PShader mShader;

  Type mType = Static;
};

#endif // Model_h__

