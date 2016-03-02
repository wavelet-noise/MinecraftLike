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
#include "RenderMeshVao.h"
#include "TextureManager.h"



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

uint32_t Render::AddModel(const std::string &mesh, const std::string &texture, const std::string &shader)
{
  mModels.push_back(std::move(std::make_unique<RenderModel>()));

  auto &model = *mModels.back();

  model.mMesh = std::make_shared<Mesh>();
  model.mTexture = std::get<0>(TextureManager::Get().GetTexture(texture));
  model.mShader = DBShaders::Get().GetShader("shaders/basic.glsl");

  return mGenId++;
}

void Render::EraseModel(size_t id)
{
  if (id >= mModels.size() || !mModels[id])
  {
    LOG(warning) << "Модуль уже удалена. id: " << id;
    return;
  }

  mModels[id].reset();
}

void Render::SetModelMatrix(size_t id, const glm::mat4 &matrix)
{
  if (id >= mModels.size() || !mModels[id])
  {
    LOG(warning) << "Установка матрицы на неизвестную модель. id: " << id;
    return;
  }
  auto &model = *mModels[id];

  model.mModel = matrix;
}

void Render::Draw(Camera &camera)
{
  for (auto &i : mModels)
  {
    if (!i)
    {
      continue;
    }
    auto &model = *i;

    const auto &aabb = model.mMesh->GetAABB();
    if (!camera.BoxWithinFrustum(model.mModel * std::get<Mesh::MinAABB>(aabb), model.mModel * std::get<Mesh::MaxAABB>(aabb)))
    {
      continue;
    }

    model.mTexture->Set(TEXTURE_SLOT_0);

    auto &shader = model.mShader;
    shader->Use();
    shader->SetUniform(TEXTURE_SLOT_0, "atlas");

    shader->SetUniform(camera.GetViewProject() * model.mModel, "transform_VP");

    model.mMesh->Draw();
  }
}

