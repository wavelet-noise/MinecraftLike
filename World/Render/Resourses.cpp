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
    return nullptr;
  }

  return it->second;
}

void Resourses::LoadMesh(const std::string &name)
{
  auto &mesh = std::make_shared<Mesh>();
  mesh->Load(name);
  mesh->BuildAABB();
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
    return nullptr;
  }

  return std::const_pointer_cast<const Texture>(it->second);
}

PCMesh Resourses::GetMesh(const std::string &name) const
{
  auto it = mMeshes.find(name);
  if (it == mMeshes.end())
  {
    return nullptr;
  }

  return std::const_pointer_cast<const Mesh>(it->second);
}
