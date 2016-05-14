#include "SplitConnectionTesselator.h"
#include "..\Render\TextureManager.h"
#include "Config.h"
#include "Tessellator.h"
#include "SectorTessellator.h"
#include "..\tools\CoordSystem.h"
#include "GameObject.h"
#include <Serealize.h>
#include <Render\Resources.h>

void SplitConnectionTesselator::SetTexture(int side, const std::string &texture)
{
	for (unsigned int i = 0; i < 7; ++i)
	{
		if (side & (1 << i))
		{
			mTextures[i] = std::get<1>(TextureManager::Get().GetTexture(texture));
		}
	}
}

SplitConnectionTesselator::SplitConnectionTesselator()
{
}

void SplitConnectionTesselator::JsonLoad(const rapidjson::Value & val)
{
	if (val.HasMember("all"))
	{
		SetTexture(ALL, val["all"].GetString());
	}
	else
		if (val.HasMember("sides"))
		{
			const rapidjson::Value &arr = val["sides"];
			SetTexture(FRONT, arr.Begin()->GetString());
			SetTexture(RIGHT, arr[1].GetString());
			SetTexture(BACK, arr[2].GetString());
			SetTexture(LEFT, arr[3].GetString());
			SetTexture(TOP, arr[4].GetString());
			SetTexture(BOTTOM, arr[5].GetString());
			SetTexture(CENTER, arr[6].GetString());
		}
		else
			LOG(error) << "missed texture";

	if (val.HasMember("connections"))
	{
		const rapidjson::Value &arr = val["connections"];
		for (int i = 0; i < 7; i++)
		{
			if (auto me = Resources::Get().GetMesh(arr[i].GetString()))
			{
				PMesh<VertexVTN> m = std::make_shared<TemplateMesh<VertexVTN>>(*me);
				const auto &uv4 = mTextures[6];
				for (int i = 0; i < m->SizeVertex(); i++)
				{
					m->Vertex(i).texture.x = glm::mix(uv4.x, uv4.z, m->Vertex(i).texture.x);
					m->Vertex(i).texture.y = glm::mix(uv4.y, uv4.w, m->Vertex(i).texture.y);
				}
				mModel[i].GetMesh() = m;
			}
		}
	}
	else
		LOG(error) << "missed model";

	BlockTessellator::JsonLoad(val);
}

PGameObjectTessellator SplitConnectionTesselator::Clone()
{
	return PGameObjectTessellator();
}

PModel SplitConnectionTesselator::GetModel(const TessellatorParams & params, int slise)
{
	static glm::ivec3 neib[] = {
		{  0, -1,  0 },	//FRONT 
		{  1,  0,  0 },	//RIGHT 
		{  0,  1,  0 },	//BACK 
		{ -1,  0,  0 },	//LEFT 
		{  0,  0,  1 },	//TOP
		{  0,  0, -1 }	//BOTTOM 
	};					

	PModel m = std::make_shared<Model>();
	m->GetMesh() = std::make_shared<TemplateMesh<VertexVTN>>();

	int i = 0;
	for (const auto &n : neib)
	{
		if (auto t = params.tesselator->GetBlock(params.wbpos + n))
		{
			if(mModel[i].GetMesh())
				m->GetMesh()->Push(*mModel[i].GetMesh());
		}
		i++;
	}

	return m;
}
