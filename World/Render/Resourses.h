#pragma once
#ifndef DBShaders_h__
#define DBShaders_h__


#include <string>
#include <unordered_map>
#include "Render\Shader.h"
#include <memory>
#include <boost\core\noncopyable.hpp>
#include "TemplateMesh.h"
#include "Texture.h"

// База данных шейдеров.
class Resourses : boost::noncopyable
{
public:
  static Resourses &Get();

  PShader GetShader(const std::string &name) const;

  // Загрузить шейдер с указанным именем.
  PShader LoadShader(const std::string &name);

  PMesh<VertexVTN> GetMesh(const std::string &name) const;

  void LoadMesh(const std::string &name);

  void LoadTexture(const std::string &name, bool mip = false, bool smooth = false, TextureDim dim = TEXTURE_DIM_2, const glm::vec3 &size = {});

  PCTexture GetTexture(const std::string &name) const;

private:
  std::unordered_map<std::string, PShader> mShaders;
  std::unordered_map<std::string, PMesh<VertexVTN>> mMeshes;
  std::unordered_map<std::string, PTexture> mTextures;
};



#endif // DBShaders_h__
