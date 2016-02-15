// ============================================================================
// ==                   Copyright (c) 2015, Smirnov Denis                    ==
// ==                  See license.txt for more information                  ==
// ============================================================================
#pragma once
#ifndef Shader_h__
#define Shader_h__

#include <memory>
#include <string>
#include <glm/glm.hpp>
#include <unordered_map>
#include "Vertex.h"

class Shader;
typedef std::shared_ptr<Shader> PShader;

/// Шейдер.
class Shader
{
public:
  Shader();
  ~Shader();

  /// Установить шейдер.
  void Use();

  /// Получить список локаций.
  std::vector<int> GetAttributeLocation(const std::vector<Attribute> &attribute);

  template<class T>
  void SetUniform(const T &val, const char *name = "")
  {
    SetUniform_(val, name);
  };

  ///
  int GetUniformLocation(const std::string &uni_name) const;

  ///
  void AddExtension(std::string s);

  ///
  void AddDefine(std::string s);

  ///
  bool Link();

  ///
  void BuildType(int type);

  ///
  void BuildBody(const std::string & filename);

#define UNIFORM_QUOTE(name) #name
#define UNIFORM_MAKE_STR(macro) UNIFORM_QUOTE(macro)
#define SetUniformNVP(val) SetUniformNVP(val, UNIFORM_MAKE_STR(val))

private:

  unsigned int mProgram;

private:

  /// Загрузить шейдер.
  unsigned int CreateShader(const std::string &data, int type);

  /// Удалить шейдер
  void DeleteShader(unsigned int shader);

  /// Записать файл
  void SaveTxtFile(const std::string & fileName, const std::string & content);

  /// Прочитать файл.
  std::string ReadTxtFile(const std::string &fileName);

  /// Записать дамп шейдера на диск
  void LogDumpError(const std::string & filename, const std::string & str, int shader);

  void BuildBody(std::stringstream &ss, const std::string &filename, int level = 0 );

private:

  void SetUniform_(const glm::mat4 &val, const std::string &uni_name) const;
  void SetUniform_(const glm::mat3 &val, const std::string &uni_name) const;
  void SetUniform_(const glm::mat2 &val, const std::string &uni_name) const;
  void SetUniform_(int val, const std::string &uni_name) const;
  void SetUniform_(unsigned int val, const std::string &uni_name) const;
  void SetUniform_(const glm::vec4 &val, const std::string &uni_name) const;
  void SetUniform_(const glm::vec3 &val, const std::string &uni_name) const;
  void SetUniform_(const glm::vec2 &val, const std::string &uni_name) const;
  void SetUniform_(const float &val, const std::string &uni_name) const;

private:

  mutable std::unordered_map<std::string, int> mUniforms;
  std::vector<int> shaders_;

  std::vector<std::string> extensions;
  std::vector<std::string> defines;

  std::string shaderfile_name;
  std::string version = "#version 330 core";
  bool source_loaded = false;
  std::string body;
};

#endif // Shader_h__
