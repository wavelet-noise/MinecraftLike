#pragma once
#ifndef DBShaders_h__
#define DBShaders_h__


#include <string>
#include <unordered_map>
#include "Render\Shader.h"
#include <memory>
#include <boost\core\noncopyable.hpp>
#include "Mesh.h"


/// База данных шейдеров.
class Resourses : boost::noncopyable
{
public:
  static Resourses &Get();

  PShader GetShader(const std::string &name) const;

  /// Загрузить шейдер с указанным именем.
  PShader LoadShader(const std::string &name);

  PMesh GetMesh(const std::string &name);

  void LoadMesh(const std::string &name);

private:
  std::unordered_map<std::string, PShader> mShaders;
  std::unordered_map<std::string, PMesh> mMesh;
};



#endif // DBShaders_h__
