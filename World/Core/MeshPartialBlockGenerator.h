// ============================================================================
// ==                   Copyright (c) 2015, Smirnov Denis                    ==
// ==                  See license.txt for more information                  ==
// ============================================================================
#pragma once
#include <vector>
#include "..\Render\TemplateMesh.h"
#include "..\Render\Vertex.h"
#include <rapidjson\document.h>

class MeshPartialBlockGenerator
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
  MeshPartialBlockGenerator();

  PTemplateMesh<VertexType> Create(Side side, float percent);

  void SetTexture(int side, const std::string &texture);

  void JsonLoad(const rapidjson::Value & val);

private:
  /// —писок координат текстур дл€ сторон куба в атласе.
  std::vector<glm::vec4> mTextures;
};