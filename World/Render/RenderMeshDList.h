// ============================================================================
// ==                   Copyright (c) 2015, Smirnov Denis                    ==
// ==                  See license.txt for more information                  ==
// ============================================================================
#pragma once
#ifndef RenderMeshGL1_h__
#define RenderMeshGL1_h__

#include "IRenderMeshStrategy.h"
#include <array>

/// Стратегия рисования меша с помощью дисплейных списков.
class RenderMeshDList : public IRenderMeshStrategy
{
public:
  RenderMeshDList();
  virtual ~RenderMeshDList();

public:

  void SetAttribute(const std::vector<Attribute> &attribute, const std::vector<int> &locations) override;

  void Send(const float *vertex, size_t vertexCount, const size_t *index, size_t indexCount) override;

  void Draw() const override;

private:
  enum AttributeType
  {
    ATTRIBUTE_UNKNOWN = 0,
    ATTRIBUTE_VERTEX = 1,
    ATTRIBUTE_COLOR = 2,
    ATTRIBUTE_TEXTURE = 3,
    ATTRIBUTE_NORMAL = 4,

    ATTRIBUTE_SIZE,
  };

  std::array<Attribute, ATTRIBUTE_SIZE> mAttribute;
  std::array<bool, ATTRIBUTE_SIZE> mEnabled;

  bool mCreated = false;
  uint32_t mList = 0;

  size_t mVertexSize = 0;
};



#endif // RenderMeshGL1_h__