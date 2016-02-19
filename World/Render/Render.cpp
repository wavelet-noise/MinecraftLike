// ============================================================================
// ==         Copyright (c) 2016, Samsonov Andrey and Smirnov Denis          ==
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
#include "DBShaders.h"
#include <assert.h>
#include "Camera.h"
#include <GLFW/glfw3.h>



Render::Render()
{
  mVersion = { -1 , -1 };
  GL_CALL(glGetIntegerv(GL_MAJOR_VERSION, &mVersion[0]));
  GL_CALL(glGetIntegerv(GL_MINOR_VERSION, &mVersion[1]));

  // Настройки для старого огл.
  if (mVersion[0] < 3)
  {
    GL_CALL(glEnable(GL_TEXTURE_2D));
  }

  GL_CALL(glEnable(GL_CULL_FACE));
  GL_CALL(glCullFace(GL_BACK));

  GL_CALL(glEnable(GL_DEPTH_TEST));            // Разрешить тест глубины
  GL_CALL(glDepthFunc(GL_LEQUAL));            // Тип теста глубины

  GL_CALL(glClearColor(117.0f / 255.0f, 187.0f / 255.0f, 253.0f / 255.0f, 1.0f));

  DBShaders::Get().LoadShader("shaders/basic.glsl");

  int ntex, texss;
  glGetIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, &ntex);
  glGetIntegerv(GL_MAX_TEXTURE_SIZE, &texss);
  LOG(info) << "Renderer: " << glGetString(GL_RENDERER);
  LOG(info) << "Vendor: " << glGetString(GL_VENDOR);
  LOG(info) << "Version: " << glGetString(GL_VERSION);
  LOG(info) << "GLSL version: " << glGetString(GL_SHADING_LANGUAGE_VERSION);
  LOG(info) << "using OpenGL: " << mVersion[0] << "." << mVersion[1];
  LOG(info) << "GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS: " << ntex;
  LOG(info) << "GL_MAX_TEXTURE_SIZE: " << texss;

  if (mVersion[0] >= 4 && mVersion[1] >= 3)
  {
    int nuni;
    glGetIntegerv(GL_MAX_UNIFORM_LOCATIONS, &nuni);
    LOG(info) << "GL_MAX_UNIFORM_LOCATIONS: " << nuni;
  }
}


Render::~Render()
{
}

void Render::Initialize()
{
  glewExperimental = true; // Needed in core profile

  GLenum error = glewInit();
  if (error != GLEW_OK)
  {
    std::cout << "glew error: " << glewGetErrorString(error) << std::endl;
    throw "GLEW not initialized.";
  }
  glGetError();

  LOG(info) << "GLEW: " << glewGetString(GLEW_VERSION);
}

RenderIterator Render::PushModel(const Model &model, const glm::mat4 &matrix)
{
  std::lock_guard<std::mutex> lock(mMutex);

  mAddList.push_back({ model, matrix });
  mAddList.back().mIterator = std::make_shared<Element::Iterator>(--mAddList.end());

  return{ mAddList.back().mIterator, mMutex };
}

void Render::Draw(Camera &camera)
{
  AddElements();
  SwapMatrix();

  for (auto &i : mDrawList)
  {
    auto aabb = i.model.GetAABB();
    if (!camera.BoxWithinFrustum(i.matrix * std::get<0>(aabb), i.matrix * std::get<1>(aabb)))
      continue;

    if(auto t = i.model.GetTexture())
      t->Set(TEXTURE_SLOT_0);

    auto &shader = i.model.GetShader();
    shader->Use();
    shader->SetUniform(TEXTURE_SLOT_0, "atlas");

    //TODO: prebuild NVP
    shader->SetUniform(camera.GetViewProject() * i.matrix, "transform_VP");

    i.model.GetMesh()->Draw();
  }
}

void Render::AddElements()
{
  std::lock_guard<std::mutex> lock(mMutex);
  if (mAddList.empty())
  {
    return;
  }

  for (auto &i : mAddList)
  {
    mDrawList.push_back(i);
    auto &element = mDrawList.back();
    *element.mIterator = --mDrawList.end();

    auto currentTime = glfwGetTime();
    element.model.GetMesh()->Compile(element.model.GetShader()->GetAttributeLocation(element.model.GetMesh()->GetAttribute()));
    LOG(trace) << "SectorCompiled: " << glfwGetTime() - currentTime;
    
    element.model.GetMesh()->Release();
  }
  mAddList.clear();
}

void Render::SwapMatrix()
{
  std::lock_guard<std::mutex> lock(mMutex);
  for (auto &i : mDrawList)
  {
    //std::swap(i.matrix, i.matrixBack);
    i.matrix = i.matrixBack;
  }
}
