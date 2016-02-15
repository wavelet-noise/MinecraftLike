// ============================================================================
// ==                   Copyright (c) 2015, Smirnov Denis                    ==
// ==                  See license.txt for more information                  ==
// ============================================================================
#pragma once
#ifndef IRenderMesh_h__
#define IRenderMesh_h__

#include "Vertex.h"
#include "..\Render\Shader.h"

class IRenderMeshStrategy
{
public:
  virtual ~IRenderMeshStrategy() {};

  /// Настроить и включить атрибут.
  /// Для каждого типа атрибута определена фиксированная локация, 
  /// из которой шейдер будет брать поток данных.
  /// Attribute::size размер атрибута в байтах.
  /// Attribute::offset смещение атрибута в байтах.
  virtual void SetAttribute(const std::vector<Attribute> &attribute, const std::vector<int> &locations) = 0;

  /// Создать сетку в видеопамяти.
  /// @param vertex указатель на начало буфера вершин.
  /// @param vertexCount количество элементов в буфере вершин.
  /// @param index указатель на начало буфера индексов.
  /// @param indexCount количество элементов в буфере индексов.
  virtual void Compile(const float *vertex, size_t vertexCount, const size_t *index, size_t indexCount) = 0;

  /// Нарисовать сетку.
  virtual void Draw() const = 0;

};


#endif // IRenderMesh_h__