#include "DBShaders.h"
#include <GL\glew.h>


DBShaders &DBShaders::Get()
{
  static DBShaders object;
  return object;
}


void DBShaders::LoadShader(const std::string &name)
{
  auto &shader = std::make_shared<Shader>();
  shader->BuildBody(name);
  shader->BuildType(GL_FRAGMENT_SHADER);
  shader->BuildType(GL_VERTEX_SHADER);
  shader->Link();

  mShaders[name] = shader;
}

PShader DBShaders::GetShader(const std::string &name) const
{
  auto it = mShaders.find(name);
  if (it == mShaders.end())
  {
    return nullptr;
  }

  return it->second;
}
