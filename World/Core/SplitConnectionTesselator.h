#pragma once
#include "BlockTessellator.h"
#include "..\Render\Model.h"
#include "TessellatorParams.h"

// Генератор модели для блока.
// Отсекает невидимые стороны блока.
class SplitConnectionTesselator : public BlockTessellator
{
public:
	SplitConnectionTesselator();
	~SplitConnectionTesselator() = default;

	void JsonLoad(const rapidjson::Value &val) override;

	PGameObjectTessellator Clone() override;

	PModel GetModel(const TessellatorParams &params) override;

private:
	Model mModel[7];
	std::array<glm::vec4, 7> mTextures;

	void SetTexture(int side, const std::string &texture);
};

REGISTER_BLOCK_TESSELLATOR(SplitConnectionTesselator)