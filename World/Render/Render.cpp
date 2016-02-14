// ============================================================================
// ==                   Copyright (c) 2015, Smirnov Denis                    ==
// ==                  See license.txt for more information                  ==
// ============================================================================
#include "Render.h"

#include <GL/glew.h>
#define GLM_FORCE_RADIANS
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include "OpenGLCall.h"
#include <tools\Log.h>

Render::Render(void)
  : mRenderList(*this)
{
  GL_CALL(glGetIntegerv(GL_MAJOR_VERSION, &mVersion.major));
  GL_CALL(glGetIntegerv(GL_MINOR_VERSION, &mVersion.minor));

  mVersion.major = 3;
  mVersion.minor = 3;

  // Настройки для старого огл.
  if (mVersion.major < 3)
  {
    GL_CALL(glEnable(GL_TEXTURE_2D));
  }

  GL_CALL(glEnable(GL_CULL_FACE));
  GL_CALL(glCullFace(GL_BACK));

  GL_CALL(glEnable(GL_DEPTH_TEST));            // Разрешить тест глубины
  GL_CALL(glDepthFunc(GL_LEQUAL));            // Тип теста глубины

  GL_CALL(glClearColor(117.0f / 255.0f, 187.0f / 255.0f, 253.0f / 255.0f, 1.0f));

  auto &s = std::make_unique<Shader>();

  s->BuildBody("shaders/basic.glsl");
  s->BuildType(GL_FRAGMENT_SHADER);
  s->BuildType(GL_VERTEX_SHADER);
  s->Link();
  s->Use();
  s->SetUniform(0, "atlas");

  mShader = std::move(s);

  int glVersion[2] = { -1, -1 };
  int ntex, nuni, texss;
  glGetIntegerv(GL_MAJOR_VERSION, &glVersion[0]);
  glGetIntegerv(GL_MINOR_VERSION, &glVersion[1]);
  glGetIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, &ntex);
  glGetIntegerv(GL_MAX_UNIFORM_LOCATIONS, &nuni);
  glGetIntegerv(GL_MAX_TEXTURE_SIZE, &texss);
  LOG(info) << "Renderer: " << glGetString(GL_RENDERER);
  LOG(info) << "Vendor: " << glGetString(GL_VENDOR);
  LOG(info) << "Version: " << glGetString(GL_VERSION);
  LOG(info) << "GLSL version: " << glGetString(GL_SHADING_LANGUAGE_VERSION);
  LOG(info) << "using OpenGL: " << glVersion[0] << "." << glVersion[1];
  LOG(info) << "GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS: " << ntex;
  LOG(info) << "GL_MAX_UNIFORM_LOCATIONS: " << nuni;
  LOG(info) << "GL_MAX_TEXTURE_SIZE: " << texss;
}

Render::~Render(void)
{
}

void Render::Initialize()
{
  glewExperimental = true; // Needed in core profile

  GLenum error = glewInit();
  if(error != GLEW_OK)
  {
    std::cout << "glew error: " << glewGetErrorString(error) << std::endl;
    throw "GLEW not initialized.";
  }
  glGetError();

  LOG(info) << "GLEW: " << glewGetString(GLEW_VERSION);
}

const Render::Version &Render::GetVersion() const
{
  return mVersion;
}

void Render::Draw(const Model &model, const glm::mat4 &mat)
{

  mRenderList.PushModel(model, mat);
}

void Render::Draw()
{
  mRenderList.Draw(*this);
}


