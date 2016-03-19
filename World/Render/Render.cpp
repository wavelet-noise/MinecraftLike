



#include "Render.h"
#include <GL/glew.h>
#define GLM_FORCE_RADIANS
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include "OpenGLCall.h"
#include <tools\Log.h>
#include "Resourses.h"
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

  //Resourses::Get().LoadTexture("", true, TEXTURE_DIM_3, { 16,16,16 });
  auto bs = Resourses::Get().LoadShader("shaders/basic.glsl");
  bs->Use();
  bs->SetUniform(TEXTURE_SLOT_0, "atlas");
  bs->SetUniform(TEXTURE_SLOT_2, "shadowmap");
  bs->SetUniform(TEXTURE_SLOT_4, "rgbtable");
  Resourses::Get().LoadTexture("data\\rgbtable.png", false, false, TEXTURE_DIM_3, {16,16,16});
  Resourses::Get().GetTexture("data\\rgbtable.png")->Set(TEXTURE_SLOT_4);

  auto bs2 = Resourses::Get().LoadShader("shaders/shadow.glsl");
  bs2->Use();
  bs2->SetUniform(TEXTURE_SLOT_0, "atlas");

  Resourses::Get().LoadMesh("data/models/selection.obj");

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

  model.mMeshes = Resourses::Get().GetMesh("data/models/selection.obj");
  model.mTexture = std::get<0>(TextureManager::Get().GetTexture(texture));
  model.mShader = Resourses::Get().GetShader(shader);

  model.Compile();

  model.mModel = glm::translate({}, glm::vec3(0, 3, 50));

  return mGenId++;
}

void Render::EraseModel(size_t id)
{
  if (id >= mModels.size() || !mModels[id])
  {
    LOG(error) << "Model not exist. id: " << id;
    return;
  }

  mModels[id].reset();
}

void Render::SetModelMatrix(size_t id, const glm::mat4 &matrix)
{
  if (id >= mModels.size() || !mModels[id])
  {
    LOG(error) << "Model not exist. id: " << id;
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

    const auto &aabb = model.mMeshes->GetAABB();
    if (!camera.BoxWithinFrustum(model.mModel * std::get<Mesh::MinAABB>(aabb), model.mModel * std::get<Mesh::MaxAABB>(aabb)))
    {
      continue;
    }

    model.mTexture->Set(TEXTURE_SLOT_0);

    auto &shader = model.mShader;
    shader->Use();

    shader->SetUniform(camera.GetViewProject() * model.mModel, "transform_VP");

    model.mMeshes->Draw();
  }
}

