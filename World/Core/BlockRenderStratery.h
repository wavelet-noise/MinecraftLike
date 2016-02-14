// ============================================================================
// ==         Copyright (c) 2016, Samsonov Andrey and Smirnov Denis          ==
// ==                  See license.txt for more information                  ==
// ============================================================================
#pragma once
#ifndef BlockRenderStratery_h__
#define BlockRenderStratery_h__

#include "IRenderBlockStrategy.h"
#include "MeshBlockGenerator.h"
#include "..\Render\Model.h"

/// √енератор модели дл€ блока.
/// ќтсекает невидимые стороны блока.
class BlockRenderStratery : public IRenderBlockStrategy
{
public:
  BlockRenderStratery();
  ~BlockRenderStratery();

  /// ƒанна€ стратеги€ всегда формирует статическую модель.
  bool IsStatic() const override
  {
    return true;
  };

  void Load(const rapidjson::Value &val) override;

  const Model &GetModel(const GameObjectParams &params) override;

private:
  static const StringIntern mRenderAgentName;

  Model mModel;
  MeshBlockGenerator mGenerator;
};

REGISTER_RENDER_STRATEGY(BlockRenderStratery)

#endif // BlockRenderStratery_h__
