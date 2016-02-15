// ============================================================================
// ==                   Copyright (c) 2015, Smirnov Denis                    ==
// ==                  See license.txt for more information                  ==
// ============================================================================
#include "Shader.h"

#include <GL/glew.h>
#include <vector>
#include <assert.h>
#include <fstream>
#include <sstream>
#include "OpenGLCall.h"
#include <map>
#include "..\tools\Log.h"
#include <regex>
#include <boost\format.hpp>

Shader::Shader()
{
  mProgram = glCreateProgram();
}

Shader::~Shader(void)
{
  while (!shaders_.empty()) {
    GL_CALL(glDeleteShader(shaders_.back()));
    LOG(trace) << "deleting shader " << std::to_string(shaders_.back());
    shaders_.pop_back();
  }
  GL_CALL(glDeleteProgram(mProgram));
  LOG(trace) << "deleting program" << mProgram;
}

void Shader::BuildType(int type)
{
  std::stringstream ss;

  static std::map<int, std::string> shader_defines = {
    std::make_pair(GL_FRAGMENT_SHADER, "_FRAGMENT_"),
    std::make_pair(GL_VERTEX_SHADER, "_VERTEX_"),
    std::make_pair(GL_GEOMETRY_SHADER, "_GEOMETRY_"),
    std::make_pair(GL_TESS_EVALUATION_SHADER, "_TESSEVAL_"),
    std::make_pair(GL_TESS_CONTROL_SHADER, "_TESSCONTROL_")
  };

  ss << version << std::endl;
  for (const auto &def : defines)
  {
    ss << "#define " << def << std::endl;
  }

  for (const auto &ext : extensions)
  {
    ss << "#extension " << ext << " : enable" << std::endl;
  }

  ss << "#define " << shader_defines[type] << std::endl;
  ss << "//end of runtime header" << std::endl;
  ss << body;

  source_loaded = true; //notify that other "define", "extension" setting is incorrect
  CreateShader(ss.str(), type);
}

std::string get_dir(const std::string &path)
{
  return path.substr(0, path.find_last_of('/') + 1);
}

std::string get_filename_headername(std::string path)
{
  std::replace(path.begin(), path.end(), '.', '_');
  std::transform(path.begin(), path.end(), path.begin(), toupper);
  return path;
}

std::string get_name(const std::string &path)
{
  return path.substr(path.find_last_of('/') + 1);
}

void Shader::LogDumpError(const std::string &filename, const std::string &str, int shader)
{
  std::string f_name;
  if (shader != -1)
  {
    f_name = (boost::format("shader_error_log_%1%_%2%.txt") %
      get_filename_headername(get_name(filename)) %
      shader).str();

    LOG(fatal) << "in file " << filename;
    LOG(fatal) << "shader error detail saveid in " << f_name;
  }
  else
  {
    f_name = (boost::format("shader_force_dump_%1%_%2%.txt") %
      get_filename_headername(get_name(filename)) %
      shader).str();

    LOG(trace) << filename << " force dump to " << f_name;
  }

  std::stringstream out_file;
  out_file << str;
  if (shader != -1)
  {
    char infoLog[1024];
    int infologLength = 0;
    glGetShaderInfoLog(shader, 1024, &infologLength, infoLog);
    out_file << std::endl << "=======ERROR======" << std::endl << infoLog << std::endl;
  }
  SaveTxtFile(f_name, out_file.str());
}

void Shader::BuildBody(const std::string &filename)
{
  std::stringstream ss;
  shaderfile_name = filename;
  BuildBody(ss, filename);

  body = ss.str();
}

void Shader::BuildBody(std::stringstream &ss, const std::string &filename, int level /*= 0 */)
{
  if (level > 32)
    LOG(fatal) << "header inclusion depth limit reached, might be caused by cyclic header inclusion";

  LOG(trace) << "build shader body for " << filename;

  static const std::regex re("^[ ]*#[ ]*include[ ]+[\"<](.*)[\">].*");
  static const std::regex rdump("^[ ]*//DUMP_SOURCE.*");
  std::stringstream input;

  input << ReadTxtFile(filename);

  size_t line_number = 1;
  std::smatch matches;

  std::string line;
  while (std::getline(input, line))
  {
    if (std::regex_search(line, matches, re))
    {
      std::string include_file = matches[1];

      ss << "#ifndef " << get_filename_headername(include_file) << std::endl;
      ss << "#define " << get_filename_headername(include_file) << std::endl;
      LOG(trace) << "include parsing " << include_file;
      BuildBody(ss, get_dir(filename) + include_file, level + 1);
      ss << "#endif //" << get_filename_headername(include_file) << std::endl;
    }
    else
    {
      ss << line << std::endl;
    }

    if (std::regex_search(line, matches, rdump))
    {
      LogDumpError(filename, ss.str(), -1);
    }
    ++line_number;
  }
}

void Shader::Use()
{
  GL_CALL(glUseProgram(mProgram));
}

std::vector<int> Shader::GetAttributeLocation(const std::vector<Attribute> &attribute)
{
  std::vector<int> locations;
  locations.reserve(attribute.size());

  for (const auto &attr : attribute)
  {
    auto location = glGetAttribLocation(mProgram, attr.name);
    locations.push_back(location);
  }

  return locations;
}

unsigned int Shader::CreateShader(const std::string &data, int type)
{
  if (data.empty())
  {
    return 0;
  }

  GLuint shader = 0;
  //GL_CALL(shader = glCreateShader(type));
  shader = glCreateShader(type);

  if (int er = glGetError())
  {
    LOG(fatal) << "glCreateShader error " << er;
    throw;
  }

  LOG(trace) << "compile type " << type;

  char const *sourcePointer = data.c_str();
  GL_CALL(glShaderSource(shader, 1, &sourcePointer, NULL));
  GL_CALL(glCompileShader(shader));
  GL_CALL(glAttachShader(mProgram, shader));

  GLint compiled = GL_FALSE;
  GL_CALL(glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled));

  if (compiled != GL_TRUE || glGetError())
  {
    LogDumpError(shaderfile_name, data, shader);

    GL_CALL(glDeleteShader(shader));
    throw;
  }

  return shader;
}

void Shader::DeleteShader(unsigned int shader)
{
  GL_CALL(glDeleteShader(shader));
}

void Shader::SaveTxtFile(const std::string &fileName, const std::string &content)
{
  std::ofstream file(fileName);
  if (file.is_open()) {
    file << content;
    file.close();
  }
  else {
    LOG(fatal) << "Failed to save file " << fileName;
  }
}

std::string Shader::ReadTxtFile(const std::string &fileName)
{
  std::string code;
  std::ifstream file(fileName, std::ios::in);
  if (file.is_open())
  {
    std::string line = "";
    while (getline(file, line))
      code += "\n" + line;
    file.close();
  }
  else {
    LOG(fatal) << "Failed to read file " << fileName;
  }

  return code;
}

bool Shader::Link() {
  glLinkProgram(mProgram);
  LOG(trace) << "Program " << std::to_string(mProgram) << " linking";
  body.clear();
  extensions.clear();
  defines.clear();
  return true;
}

int Shader::GetUniformLocation(const std::string &uni_name) const
{
  const auto &search = mUniforms.find(uni_name);
  if (search == mUniforms.end())
  {
    int loc = glGetUniformLocation(mProgram, uni_name.c_str());
    if (loc == -1)
    {
      LOG(error) << uni_name << " missed in " << shaderfile_name;
    }
    mUniforms[uni_name] = loc;
    return loc;
  }
  else
  {
    return mUniforms[uni_name];
  }
}

void Shader::AddExtension(std::string s)
{
  if (source_loaded)
    LOG(fatal) << "extensions must be added before source loading";
  extensions.push_back(std::move(s));
}

void Shader::AddDefine(std::string s)
{
  if (source_loaded)
    LOG(fatal) << "defines must be added before source loading";
  defines.push_back(std::move(s));
}

void Shader::SetUniform_(const glm::mat4 &val, const std::string &uni_name) const
{
  glUniformMatrix4fv(GetUniformLocation(uni_name), 1, GL_FALSE, &val[0][0]);
}

void Shader::SetUniform_(const glm::mat3 &val, const std::string &uni_name) const
{
  glUniformMatrix3fv(GetUniformLocation(uni_name), 1, GL_FALSE, &val[0][0]);
}

void Shader::SetUniform_(const glm::mat2 &val, const std::string &uni_name) const
{
  glUniformMatrix2fv(GetUniformLocation(uni_name), 1, GL_FALSE, &val[0][0]);
}

void Shader::SetUniform_(int val, const std::string &uni_name) const
{
  glUniform1i(GetUniformLocation(uni_name), val);
}

void Shader::SetUniform_(unsigned int val, const std::string &uni_name) const
{
  glUniform1ui(GetUniformLocation(uni_name), val);
}

void Shader::SetUniform_(const glm::vec4 &val, const std::string &uni_name) const
{
  glUniform4fv(GetUniformLocation(uni_name), 1, &val[0]);
}

void Shader::SetUniform_(const glm::vec3 &val, const std::string &uni_name) const
{
  glUniform3fv(GetUniformLocation(uni_name), 1, &val[0]);
}

void Shader::SetUniform_(const glm::vec2 &val, const std::string &uni_name) const
{
  glUniform2fv(GetUniformLocation(uni_name), 1, &val[0]);
}

void Shader::SetUniform_(const float &val, const std::string &uni_name) const
{
  glUniform1f(GetUniformLocation(uni_name), val);
}