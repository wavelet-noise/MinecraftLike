#pragma once
#include "BlockTessellator.h"
#include "..\Render\Model.h"
#include "TessellatorParams.h"

// Генератор модели для блока.
// Отсекает невидимые стороны блока.
class ModelTessellator : public BlockTessellator
{
public:
	ModelTessellator();
	~ModelTessellator() = default;

	void JsonLoad(const rapidjson::Value &val) override;

	PGameObjectTessellator Clone() override;

	PModel GetModel(const TessellatorParams &params, int slise = 9999) override;

private:
	PModel mModel;
	std::array<glm::vec4, 7> mTextures;

	void SetTexture(int side, const std::string &texture);
};

REGISTER_BLOCK_TESSELLATOR(ModelTessellator)