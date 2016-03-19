



#pragma once
#ifndef TemplateMesh_h__
#define TemplateMesh_h__

#include <memory>
#include <vector>
#include "RenderMeshDList.h"
#include "RenderMeshVao.h"
#include <Serealize.h>

template<class V>
class TemplateMesh;

template<class V>
using PTemplateMesh = std::shared_ptr<TemplateMesh<V>>;

template<class V>
class TemplateMesh
{
public:
  using VertexType = V;

  inline TemplateMesh()
  {
    mStrategy = std::make_unique<RenderMeshVao>();
    //mStrategy = std::make_unique<RenderMeshDList>();
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

  void Push(const TemplateMesh<VertexType> &mesh)
  {
    mIndex.reserve(mesh.mIndex.size());
    size_t size = mVertex.size();
    for (const auto &i : mesh.mIndex)
    {
      mIndex.push_back(size + i);
    }

    mVertex.reserve(mVertex.size() + mesh.SizeVertex());
    mVertex.insert(mVertex.end(), mesh.mVertex.begin(), mesh.mVertex.end());
  }

  /// Создать сетку в видеопамяти.
  /// @param vertex указатель на начало буфера вершин.
  /// @param vertexSize количество элементов в буфере вершин.
  /// @param index указатель на начало буфера индексов.
  /// @param indexSize количество элементов в буфере индексов.
  inline void Compile(const std::vector<int> &locations)
  {
    mStrategy->SetAttribute(GetAttribute(), locations);
    mStrategy->Send(reinterpret_cast<float *>(mVertex.data()), mVertex.size(),
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

  inline const std::vector<Attribute> &GetAttribute() const noexcept
  {
    return VertexType::Get();
  }

  void JsonLoad(const rapidjson::Value &val)
  {
    JSONLOAD(sge::make_nvp("vertex", mVertex), sge::make_nvp("index", mIndex));
  }

private:

  std::vector<VertexType> mVertex;
  std::vector<size_t> mIndex;

  std::unique_ptr<IRenderMeshStrategy> mStrategy;
};



#endif // TemplateMesh_h__


