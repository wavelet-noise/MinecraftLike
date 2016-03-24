#include "Resourses.h"
#include <GL\glew.h>


Resourses &Resourses::Get()
{
	static Resourses object;
	return object;
}


PShader Resourses::LoadShader(const std::string &name)
{
	auto &shader = std::make_shared<Shader>();
	shader->BuildBody(name);
	shader->BuildType(GL_FRAGMENT_SHADER);
	shader->BuildType(GL_VERTEX_SHADER);
	shader->Link();

	mShaders[name] = shader;
	return shader;
}

PShader Resourses::GetShader(const std::string &name) const
{
	auto it = mShaders.find(name);
	if (it == mShaders.end())
	{
		LOG(fatal) << name << " shader missed";
		return nullptr;
	}

	return it->second;
}

void Resourses::LoadMesh(const std::string &name)
{
	auto &mesh = std::make_shared<TemplateMesh<VertexVTN>>();
	mesh->Load(name, &VertexVTN::vertex, &VertexVTN::normal, &VertexVTN::texture);
	mesh->BuildAABB(&VertexVTN::vertex);
	mMeshes[name] = mesh;
}

void Resourses::LoadTexture(const std::string & name, bool mip, bool smooth, TextureDim dim, const glm::vec3 &size)
{
	auto &b = std::make_shared<Bitmap>(name);
	auto &tex = std::make_shared<Texture>(*b, mip, smooth, dim, size);
	mTextures[name] = tex;
}

PCTexture Resourses::GetTexture(const std::string & name) const
{
	auto it = mTextures.find(name);
	if (it == mTextures.end())
	{
		LOG(fatal) << name << " texture missed";
		return nullptr;
	}

	return std::const_pointer_cast<const Texture>(it->second);
}

PMesh<VertexVTN> Resourses::GetMesh(const std::string &name) const
{
	auto it = mMeshes.find(name);
	if (it == mMeshes.end())
	{
		LOG(fatal) << name << " mesh missed";
		return nullptr;
	}

	return std::const_pointer_cast<TemplateMesh<VertexVTN>>(it->second);
}
