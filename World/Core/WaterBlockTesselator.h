// ============================================================================
// ==         Copyright (c) 2016, Samsonov Andrey and Smirnov Denis          ==
// ==                  See license.txt for more information                  ==
// ============================================================================
#pragma once
#include "MeshPartialBlockGenerator.h"
#include "BlockTessellator.h"
#include "..\Render\Model.h"
#include "TessellatorParams.h"

class Water;

/// Генератор модели для блока.
/// Отсекает невидимые стороны блока.
class WaterBlockTessellator : public BlockTessellator
{
public:
  WaterBlockTessellator();
  ~WaterBlockTessellator() = default;

  void JsonLoad(const rapidjson::Value &val) override;

  PGameObjectTessellator Clone() override;

  const Model &GetModel(const TessellatorParams &params) override;

  void SayDepth(float d);

private:
  friend Water;
  Model mModel;
  MeshPartialBlockGenerator mGenerator;
  float percent = 0.5;
};

REGISTER_BLOCK_TESSELLATOR(WaterBlockTessellator)