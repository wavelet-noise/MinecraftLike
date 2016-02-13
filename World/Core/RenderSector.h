// ============================================================================
// ==         Copyright (c) 2016, Samsonov Andrey and Smirnov Denis          ==
// ==                  See license.txt for more information                  ==
// ============================================================================
#pragma once
#ifndef RenderSector_h__
#define RenderSector_h__

#include <atomic>
#include "..\Render\Render.h"
#include <glm/glm.hpp>
#include "..\tools\CoordSystem.h"


class RenderSector
{
public:
  RenderSector(const SPos &pos);
  ~RenderSector();

  /// Сообщить, что геометрия изменилась.
  void SayChanged();

  /// Нужно ли перестроить сектор?
  bool IsNeedBuild() const;

  void Push(const Model &model, const WPos &pos);

  void Update(Render &render);
private:
  Model mModel;
  glm::mat4 mModelMatrix;

  bool mIsChanged = true;
  bool mIsNeedBuild = false;
};



#endif // RenderSector_h__