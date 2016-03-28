




#include "Model.h"
#include "Resources.h"
#include <Serealize.h>
#include"TextureManager.h"

Model::Model()
{
  mShader = Resources::Get().GetShader("shaders/basic.glsl");
}

void Model::JsonLoad(const rapidjson::Value & val)
{

}

void Model::BuildAABB(glm::vec3 VertexVTN::* p)
{
	mMeshes->BuildAABB(p);
}

bool Model::IsAabbDot()
{
	return mMeshes->IsAabbDot();
}

void DeltaModel::Init(std::vector<PMeshType> & meshes)
{
	mMeshes = std::make_shared<TemplateMesh<VertexVTN>>();
	parts.reserve(meshes.size());

	mMeshes->Reserve(10000, 10000);
	for (const auto &m : meshes)
	{
		if (m)
		{
			GpuReference gr;
			gr.ibegin = mMeshes->SizeIndex();
			gr.vbegin = mMeshes->SizeVertex();
			gr.isize = m->SizeIndex();
			gr.vsize = m->SizeVertex();

			mMeshes->Push(*m);
			parts.emplace_back(gr);
		}
		else
		{
			parts.emplace_back();
		}
	}
}
