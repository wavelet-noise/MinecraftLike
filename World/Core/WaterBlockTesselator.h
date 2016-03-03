// ============================================================================
// ==         Copyright (c) 2016, Samsonov Andrey and Smirnov Denis          ==
// ==                  See license.txt for more information                  ==
// ============================================================================
#pragma once
#include "MeshBlockGenerator.h"
#include "BlockTessellator.h"
#include "..\Render\Model.h"
#include "TessellatorParams.h"

/// ��������� ������ ��� �����.
/// �������� ��������� ������� �����.
class WaterBlockTessellator : public BlockTessellator
{
public:
  WaterBlockTessellator();
  ~WaterBlockTessellator() = default;

  void JsonLoad(const rapidjson::Value &val) override;

  PGameObjectTessellator Clone() override;

  const Model &GetModel(const TessellatorParams &params) override;

private:
  Model mModel;
  MeshBlockGenerator mGenerator;
  float percent = 1;
};

REGISTER_BLOCK_TESSELLATOR(WaterBlockTessellator)