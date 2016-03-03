// ============================================================================
// ==                   Copyright (c) 2015, Smirnov Denis                    ==
// ==                  See license.txt for more information                  ==
// ============================================================================
#pragma once
#ifndef ModelBlockGenerator_h__
#define ModelBlockGenerator_h__

#include <vector>
#include "..\Render\TemplateMesh.h"
#include "..\Render\Vertex.h"
#include <rapidjson\document.h>

/// Генератор модели для блока.
/// Меш формируется автоматически, для каждой стороны блока можно указать текстуру.
/// Все текстуры должны находиться в одном текстурном атласе.
/// Для каждой стороны блока можно указать, нужно ли рисовать сторону. 
class MeshBlockGenerator
{
public:
  using VertexType = VertexVTN;

  enum Side
  {
    EMPTY = 0,

    FRONT = 1 << 0,
    RIGHT = 1 << 1,
    BACK = 1 << 2,
    LEFT = 1 << 3,
    TOP = 1 << 4,
    BOTTOM = 1 << 5,

    ALL = 0x3F,
  };

public:
  MeshBlockGenerator();

  void Create(TemplateMesh<VertexType> &mesh, Side side);

  void SetTexture(int side, const std::string &texture);

  void Generate();

  void JsonLoad(const rapidjson::Value & val);

  inline const PTemplateMesh<VertexType> &Create(Side side) const
  {
    return mMeshArray[side];
  }

private:
  /// Список координат текстур для сторон куба в атласе.
  std::vector<glm::vec4> mTextures;

  std::vector<PTemplateMesh<VertexType>> mMeshArray;
};



#endif // ModelBlockGenerator_h__