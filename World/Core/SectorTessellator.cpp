



#include "SectorTessellator.h"

#include <GLFW\glfw3.h>
#include <glm\gtc\matrix_transform.hpp>
#include <type_traits>
#include "Tessellator.h"
#include "TessellatorParams.h"
#include "..\Render\RenderSector.h"
#include "..\tools\Log.h"
#include <Render\Resources.h>

namespace
{
	Model mModel;
}

SectorTessellator::SectorTessellator(const SPos &pos)
	: mPos(pos)
{

}

void SectorTessellator::SayChanged()
{
	mChanged = true;
}

bool SectorTessellator::Update(Tessellator *tesselator, RenderSector &render, int slise)
{
	if (!mChanged || mBlocks.empty())
	{
		return false;
	}

	mModel.GetMesh()->Clear();

	auto currentTime = glfwGetTime();
	TessellatorParams params{ tesselator, this, mPos,{} };

	for (int k = 0; k < SECTOR_SIZE; k++)
	{
		if (k + mPos.z * SECTOR_SIZE <= slise)
			for (int i = 0; i < SECTOR_SIZE; i++)
				for (int j = 0; j < SECTOR_SIZE; j++)
				{
					params.wbpos = cs::SBtoWB({ i,j,k }, mPos);
					auto ind = cs::SBtoI({ i,j,k }) ;
					if (mBlocks[ind])
					{
						__PushMmodel(*GetBlock(ind)->GetModel(params, slise), { i,j,k });
					}
				}
	}

	using MeshType = std::remove_reference_t<decltype(mModel.GetMesh())>::element_type;

	if (!mModel.GetMesh()->IsEmpty())
	{
		mModel.GetMesh()->BuildAABB(&VertexVTN::vertex);
		render.Push(mModel, mPos);
	}
	else
	{
		render.Remove(mPos);
	}

	mChanged = false;
	return true;
}

void SectorTessellator::Init()
{
	using MeshType = std::remove_reference_t<decltype(mModel.GetMesh())>::element_type;
	mModel.GetMesh() = std::make_shared<MeshType>();
	mModel.GetMesh()->Reserve(1000000, 4000000);
	mModel.GetShader() = Resources::Get().GetShader("shaders/basic.glsl");
}

void SectorTessellator::__PushMmodel(const Model &model, const SBPos &pos)
{
	auto &dst = mModel.GetMesh();
	const auto &src = model.GetMesh();


	if (dst->IsEmpty())
	{
		mModel.SetTexture(model.GetTexture());
	}
	//if (!model.GetTexture() /*костыль*/ || mModel.GetTexture() == model.GetTexture())
	{
		size_t size = dst->SizeVertex();
		dst->Push(*src);
		for (size_t i = size; i < dst->SizeVertex(); ++i)
		{
			dst->Vertex(i).vertex += pos;
		}
	}
	//else
	{
		//	LOG(warning) << "Батчинг меша в секторе пропущен. Текстуры не совпадают.";
	}
}
