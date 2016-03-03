// ============================================================================
// ==         Copyright (c) 2016, Samsonov Andrey and Smirnov Denis          ==
// ==                  See license.txt for more information                  ==
// ============================================================================
#pragma once
#ifndef Mesh_h__
#define Mesh_h__


#include <vector>
#include "Vertex.h"
#include "IRenderMeshStrategy.h"
#include <memory>
#include <tuple>

using PMesh = std::shared_ptr<class Mesh>;

// TODO: Формирование данного меша из шаблонного.
class Mesh
{
public:
  enum
  {
    MinAABB,
    MaxAABB,
  };
  using VertexFormat = std::vector<Attribute>;
  using AABB = std::tuple<glm::vec4, glm::vec4>;

public:
  /// Скомпилировать меш для данного шейдера.
  /// После компиляции данные о вершинах в ОЗУ освобождаются.
  void Compile(class Shader &shader);

  void Draw();

  inline const VertexFormat &GetAttribute() const noexcept
  {
    return mAttribute;
  }

  inline const AABB &GetAABB() const noexcept
  {
    return mAABB;
  }

  void Load(const std::string &s);

private:
  std::vector<float> mVertex;
  std::vector<size_t> mIndex;

  VertexFormat mAttribute;

  std::unique_ptr<IRenderMeshStrategy> mStrategy;

  AABB mAABB;

private: 
  void BuildAABB(const Attribute &attributeVertex);

};



#endif // Mesh_h__
