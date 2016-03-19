



#include "SplitBlockTessellator.h"
#include "..\Render\TextureManager.h"
#include "Config.h"
#include "Tessellator.h"
#include "SectorTessellator.h"
#include "..\tools\CoordSystem.h"
#include "GameObject.h"
#include <Serealize.h>

SplitBlockTessellator::SplitBlockTessellator()
{
	mModel.SetTexture(std::get<0>(TextureManager::Get().GetTexture("stone")));//TODO:remove
}


const Model & SplitBlockTessellator::GetModel(const TessellatorParams &params)
{
	const int32_t size = static_cast<int32_t>(SECTOR_SIZE);

	size_t sides = MeshPartialBlockGenerator::ALL;
	auto pos = cs::WBtoSB(params.wbpos, params.spos);
	if (pos.x < size - 1)
	{
		++pos.x;
		if (auto block = params.sector->GetBlock(pos))
		{
			if (!block->IsTransparent())
			{
				sides &= ~MeshPartialBlockGenerator::RIGHT;
			}
		}
		--pos.x;
	}
	else
	{
		auto pos = static_cast<WBPos>(params.wbpos);
		++pos.x;
		if (auto block = params.tesselator->GetBlock(pos))
		{
			if (!block->IsTransparent())
			{
				sides &= ~MeshPartialBlockGenerator::RIGHT;
			}
		}
	}
	if (pos.x > 0)
	{
		--pos.x;
		if (auto block = params.sector->GetBlock(pos))
		{
			if (!block->IsTransparent())
			{
				sides &= ~MeshPartialBlockGenerator::LEFT;
			}
		}
		++pos.x;
	}
	else
	{
		auto pos = static_cast<WBPos>(params.wbpos);
		--pos.x;
		if (auto block = params.tesselator->GetBlock(pos))
		{
			if (!block->IsTransparent())
			{
				sides &= ~MeshPartialBlockGenerator::LEFT;
			}
		}
	}

	if (pos.y < size - 1)
	{
		++pos.y;
		if (auto block = params.sector->GetBlock(pos))
		{
			if (!block->IsTransparent())
			{
				sides &= ~MeshPartialBlockGenerator::BACK;
			}
		}
		--pos.y;
	}
	else
	{
		auto pos = static_cast<WBPos>(params.wbpos);
		++pos.y;
		if (auto block = params.tesselator->GetBlock(pos))
		{
			if (!block->IsTransparent())
			{
				sides &= ~MeshPartialBlockGenerator::BACK;
			}
		}
	}
	if (pos.y > 0)
	{
		--pos.y;
		if (auto block = params.sector->GetBlock(pos))
		{
			if (!block->IsTransparent())
			{
				sides &= ~MeshPartialBlockGenerator::FRONT;
			}
		}
		++pos.y;
	}
	else
	{
		auto pos = static_cast<WBPos>(params.wbpos);
		--pos.y;
		if (auto block = params.tesselator->GetBlock(pos))
		{
			if (!block->IsTransparent())
			{
				sides &= ~MeshPartialBlockGenerator::FRONT;
			}
		}
	}

	if (pos.z < size - 1)
	{
		++pos.z;
		if (auto block = params.sector->GetBlock(pos))
		{
			if (!block->IsTransparent())
			{
				sides &= ~MeshPartialBlockGenerator::TOP;
			}
		}
		--pos.z;
	}
	else
	{
		auto pos = static_cast<WBPos>(params.wbpos);
		++pos.z;
		if (auto block = params.tesselator->GetBlock(pos))
		{
			if (!block->IsTransparent())
			{
				sides &= ~MeshPartialBlockGenerator::TOP;
			}
		}
	}
	if (pos.z > 0)
	{
		--pos.z;
		if (auto block = params.sector->GetBlock(pos))
		{
			if (!block->IsTransparent())
			{
				sides &= ~MeshPartialBlockGenerator::BOTTOM;
			}
		}
		++pos.z;
	}
	else
	{
		auto pos = static_cast<WBPos>(params.wbpos);
		--pos.z;
		if (auto block = params.tesselator->GetBlock(pos))
		{
			if (!block->IsTransparent())
			{
				sides &= ~MeshPartialBlockGenerator::BOTTOM;
			}
		}
	}

	static auto ofs = std::vector<glm::ivec3>{ { -1, -1, -1 }, {  1, -1, -1 }, { -1,  1, -1 }, { -1, -1,  1 },
											   {  1, -1,  1 }, {  1,  1, -1 }, { -1,  1,  1 }, {  1,  1,  1 } };

	std::array<char, 8> a;

	for (int i = 0; i < 8; i++)
	{
		a[i] = 1;

		glm::ivec3 npos[] = { pos + ofs[i],  pos + glm::ivec3{ ofs[i].x, 0, ofs[i].z }, pos + glm::ivec3(0, ofs[i].y, ofs[i].z) };
		for (int j = 0; j < 3; j++)
		if (npos[j].x > 0 && npos[j].y > 0 && npos[j].z > 0 && npos[j].z < size - 1 && npos[j].x < size - 1 && npos[j].y < size - 1)
		{
			if (auto block = params.sector->GetBlock(npos[j]))
			{
				if (!block->IsTransparent())
					a[i]++;
			}
		}
	}

	mModel.GetMesh() = mGenerator.Create(static_cast<MeshPartialBlockGenerator::Side>(sides), a);

	return mModel;
}

void SplitBlockTessellator::JsonLoad(const rapidjson::Value & val)
{
	JSONLOAD(sge::make_nvp("model", mModel), sge::make_nvp("generator", mGenerator));
	BlockTessellator::JsonLoad(val);
}

PGameObjectTessellator SplitBlockTessellator::Clone()
{
	return MakeBlockTessellator<SplitBlockTessellator>(*this);
}
