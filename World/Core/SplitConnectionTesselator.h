#pragma once
#include "BlockTessellator.h"
#include "..\Render\Model.h"
#include "TessellatorParams.h"

// Генератор модели для блока.
// Отсекает невидимые стороны блока.
class SplitConnectionTesselator : public BlockTessellator
{
	enum Side
	{
		EMPTY = 0,

		FRONT = 1 << 0,
		RIGHT = 1 << 1,
		BACK = 1 << 2,
		LEFT = 1 << 3,
		TOP = 1 << 4,
		BOTTOM = 1 << 5,
		CENTER = 1 << 6,

		ALL = 0b01111111,
	};

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