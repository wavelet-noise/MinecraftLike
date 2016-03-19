



#pragma once
#ifndef BlockRenderStratery_h__
#define BlockRenderStratery_h__

#include "MeshPartialBlockGenerator.h"
#include "BlockTessellator.h"
#include "..\Render\Model.h"
#include "TessellatorParams.h"

/// Генератор модели для блока.
/// Отсекает невидимые стороны блока.
class SplitBlockTessellator : public BlockTessellator
{
public:
  SplitBlockTessellator();
  ~SplitBlockTessellator() = default;

  void JsonLoad(const rapidjson::Value &val) override;

  PGameObjectTessellator Clone() override;

  const Model &GetModel(const TessellatorParams &params) override;

private:
  Model mModel;
  MeshPartialBlockGenerator mGenerator;
};

REGISTER_BLOCK_TESSELLATOR(SplitBlockTessellator)

#endif // BlockRenderStratery_h__
