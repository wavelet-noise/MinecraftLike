



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

bool SectorTessellator::Update(Tessellator *tesselator, RenderSector &render)
{
	if (!mChanged)
	{
		return false;
	}

	mModel.GetMesh()->Clear();

	auto currentTime = glfwGetTime();
	TessellatorParams params{ tesselator, this, mPos,{} };

	for (size_t i = 0; i < mBlocks.size(); ++i)
	{
		if (mBlocks[i])
		{
			auto sbpos = cs::ItoSB(i);
			params.wbpos = cs::SBtoWB(sbpos, mPos);
			__PushMmodel(*GetBlock(i)->GetModel(params), sbpos);
		}
	};

	//LOG(trace) << "SectorTessellated: " << glfwGetTime() - currentTime;
	//LOG(trace) << "SectorTessellated: [" << mPos.x << "," << mPos.y << "," << mPos.z << "]";

	using MeshType = std::remove_reference_t<decltype(mModel.GetMesh())>::element_type;

	if (!mModel.GetMesh()->IsEmpty())
	{
		mModel.GetMesh()->BuildAABB(&VertexVTN::vertex);
		render.Push(mModel, mPos);
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
