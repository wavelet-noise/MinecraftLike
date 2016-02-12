// ============================================================================
// ==         Copyright (c) 2016, Samsonov Andrey and Smirnov Denis          ==
// ==                  See license.txt for more information                  ==
// ============================================================================
#pragma once
#ifndef Mesh_h__
#define Mesh_h__

#include <memory>
#include <vector>
#include "RenderMeshDList.h"
#include "RenderMeshVao.h"

template<class V>
class Mesh;

template<class V>
using PMesh = std::shared_ptr<Mesh<V>>;

template<class V>
class Mesh
{
public:
  using VertexType = V;

  inline Mesh()
  {
    mStrategy = std::make_unique<RenderMeshVao>();
    mStrategy = std::make_unique<RenderMeshDList>();
  }

  inline VertexType &Vertex(size_t i)
  {
    return mVertex[i];
  }

  inline size_t &Index(size_t i)
  {
    return mIndex[i];
  }

  inline void Reserve(size_t vertexCount, size_t indexCount)
  {
    mVertex.reserve(vertexCount);
    mIndex.reserve(indexCount);
  }

  inline void Clear()
  {
    mVertex.clear();
    mIndex.clear();
  }

  inline void Release()
  {
    mVertex.swap(decltype(mVertex)());
    mIndex.swap(decltype(mIndex)());
  }

  inline size_t SizeVertex() const
  {
    return mVertex.size();
  }

  inline size_t SizeIndex() const
  {
    return mIndex.size();
  }

  inline void PushVertex(const VertexType &vertex)
  {
    mVertex.push_back(vertex);
  }

  inline void PushIndex(size_t index)
  {
    mIndex.push_back(index);
  }

  inline bool Empty() const noexcept
  {
    return mVertex.empty() && mIndex.empty();
  }

  void Push(const Mesh<VertexType> &mesh)
  {
    mIndex.reserve(mesh.mIndex.size());
    size_t size = mVertex.size();
    for (const auto &i : mesh.mIndex)
    {
      mIndex.push_back(size + i);
    }

    mVertex.insert(mVertex.end(), mesh.mVertex.begin(), mesh.mVertex.end());
  }

  /// Создать сетку в видеопамяти.
  /// @param vertex указатель на начало буфера вершин.
  /// @param vertexSize количество элементов в буфере вершин.
  /// @param index указатель на начало буфера индексов.
  /// @param indexSize количество элементов в буфере индексов.
  inline void Compile()
  {
    mStrategy->SetAttribute(VertexType::Get());
    mStrategy->Compile(reinterpret_cast<float *>(mVertex.data()), mVertex.size(),
      reinterpret_cast<size_t *>(mIndex.data()), mIndex.size());
  }

  /// Нарисовать сетку.
  inline void Draw()
  {
    mStrategy->Draw();
  }

  IRenderMeshStrategy &GetStrategy()
  {
    return *mStrategy;
  }

private:
  std::vector<VertexType> mVertex;
  std::vector<size_t> mIndex;

  std::unique_ptr<IRenderMeshStrategy> mStrategy;
};



#endif // Mesh_h__