// ============================================================================
// ==         Copyright (c) 2016, Samsonov Andrey and Smirnov Denis          ==
// ==                  See license.txt for more information                  ==
// ============================================================================
#pragma once
#ifndef SectorTessellator_h__
#define SectorTessellator_h__


#include "Config.h"
#include "BlockTessellator.h"
#include <array>
#include "..\tools\CoordSystem.h"
#include "..\Render\Model.h"
#include <glm/glm.hpp>


class SectorTessellator
{
public:
  SectorTessellator(const SPos &pos);
  ~SectorTessellator();

  /// ”становить тессел€тор в блок.
  void SetBlock(const SBPos &pos, PBlockTessellator tesselator);

  /// ѕолучить тессел€тор блока.
  PBlockTessellator GetBlock(const SBPos &pos);

  /// —ообщить сектору, что он изменен.
  void SayChanged();

  /// ≈сли сектор был изменен, обновл€ет геометрию и посылает в рендер.
  void Update(class Tessellator *tesselator, class Render &render);

private:
  std::array<PBlockTessellator, SECTOR_SIZE * SECTOR_SIZE * SECTOR_SIZE> mBlocks;

  SPos mPos;

  Model mModel;
  glm::mat4 mModelMatrix;

  bool mChanged = false;

private:

  void Push(const Model &model, const WPos &pos);

};



#endif // SectorTessellator_h__