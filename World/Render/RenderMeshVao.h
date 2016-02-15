// ============================================================================
// ==         Copyright (c) 2016, Samsonov Andrey and Smirnov Denis          ==
// ==                  See license.txt for more information                  ==
// ============================================================================
#pragma once
#ifndef RenderMeshVao_h__
#define RenderMeshVao_h__


#include "IRenderMeshStrategy.h"
#include <array>

/// Стратегия рисования меша с помощью дисплейных списков.
class RenderMeshVao : public IRenderMeshStrategy
{
public:
  RenderMeshVao();
  virtual ~RenderMeshVao();

public:

  void SetAttribute(const std::vector<Attribute> &attribute, const std::vector<int> &locations) override;

  void Compile(const float *vertex, size_t vertexCount, const size_t *index, size_t indexCount) override;

  void Draw() const override;

private:
  void Create();

private:
  bool mCreated = false;
  size_t mVertexSize = 0;
  size_t mIndexCount = 0;

  unsigned int mVao = 0;
  unsigned int mVbo = 0;
  unsigned int mVbi = 0;

  const Shader *mShader = nullptr;
};



#endif // RenderMeshVao_h__