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
#include "SectorBase.h"


class SectorTessellator : public SectorBase<PGameObjectTessellator>
{
public:
  SectorTessellator(const SPos &pos);

  /// —ообщить сектору, что он изменен.
  void SayChanged();

  /// ≈сли сектор был изменен, обновл€ет геометрию и посылает в рендер.
  void Update(class Tessellator *tesselator, class RenderSector &render);

private:
  SPos mPos;

  Model mModel;

  bool mChanged = false;

private:

  void Push(const Model &model, const SBPos &pos);

};



#endif // SectorTessellator_h__