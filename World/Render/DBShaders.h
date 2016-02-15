#pragma once
#ifndef DBShaders_h__
#define DBShaders_h__


#include <string>
#include <unordered_map>
#include "Render\Shader.h"
#include <memory>
#include <boost\core\noncopyable.hpp>


/// База данных шейдеров.
class DBShaders : boost::noncopyable
{
public:
  static DBShaders &Get();

  PShader GetShader(const std::string &name) const;

  /// Загрузить шейдер с указанным именем.
  void LoadShader(const std::string &name);

private:
  std::unordered_map<std::string, PShader> mShaders;

};



#endif // DBShaders_h__
