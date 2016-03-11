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
  mMesh[name] = mesh;
}

PCMesh Resourses::GetMesh(const std::string &name) const
{
  auto it = mMesh.find(name);
  if (it == mMesh.end())
  {
    return nullptr;
  }

  return std::const_pointer_cast<const Mesh>(it->second);
}
