#include "ModelTessellator.h"
#include "..\Render\TextureManager.h"
#include "Config.h"
#include "Tessellator.h"
#include "SectorTessellator.h"
#include "..\tools\CoordSystem.h"
#include "GameObject.h"
#include <Serealize.h>
#include <Render\Resourses.h>

void ModelTessellator::SetTexture(int side, const std::string &texture)
{
	for (unsigned int i = 0; i < 6; ++i)
	{
		if (side & (1 << i))
		{
			mTextures[i] = std::get<1>(TextureManager::Get().GetTexture(texture));
		}
	}
}

ModelTessellator::ModelTessellator()
{
	mModel.SetTexture(std::get<0>(TextureManager::Get().GetTexture("stone")));
}

void ModelTessellator::JsonLoad(const rapidjson::Value & val)
{
	if (val.HasMember("all"))
	{
		SetTexture(ModelTessellator::ALL, val["all"].GetString());
	}
	else
		if (val.HasMember("sides"))
		{
			const rapidjson::Value &arr = val["sides"];
			SetTexture(ModelTessellator::FRONT, arr.Begin()->GetString());
			SetTexture(ModelTessellator::RIGHT, arr[1].GetString());
			SetTexture(ModelTessellator::BACK, arr[2].GetString());
			SetTexture(ModelTessellator::LEFT, arr[3].GetString());
			SetTexture(ModelTessellator::TOP, arr[4].GetString());
			SetTexture(ModelTessellator::BOTTOM, arr[5].GetString());
		}
		else
			LOG(error) << "missed texture";

	if (val.HasMember("model"))
	{
		const std::string &mo = val["model"].GetString();
		mModel.GetMesh() = Resourses::Get().GetMesh(mo);
	}
	else
		LOG(error) << "missed model";

	BlockTessellator::JsonLoad(val);
}

PGameObjectTessellator ModelTessellator::Clone()
{
	return PGameObjectTessellator();
}

const Model & ModelTessellator::GetModel(const TessellatorParams & params)
{
	return mModel;
}
