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

Render::Render(void)
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
}

const Render::Version &Render::GetVersion() const
{
  return mVersion;
}

void Render::Draw(const Model &model, const glm::mat4 &mat)
{
  assert(mCamera && mShader);
  mShader->Use();
  if (model.GetTexture())
  {
    model.GetTexture()->Set(TEXTURE_SLOT_0);
  }

  //TODO: prebuild NVP
  mShader->SetUniform(mCamera->GetProject() * mCamera->GetView() * mat, "transform_VP");

  model.GetMesh()->Draw();
}



