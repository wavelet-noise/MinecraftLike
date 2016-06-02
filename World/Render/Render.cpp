



#include "Render.h"
#include <GL/glew.h>
#define GLM_FORCE_RADIANS
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

#include <tools\Log.h>
#include "Resources.h"
#include <assert.h>
#include "Camera.h"
#include <GLFW/glfw3.h>
#include "RenderMeshVao.h"
#include "TextureManager.h"
#include <Core\SectorTessellator.h>


Render::Render()
{
  mVersion = { -1 , -1 };
  glGetIntegerv(GL_MAJOR_VERSION, &mVersion[0]);
  glGetIntegerv(GL_MINOR_VERSION, &mVersion[1]);

  glEnable(GL_CULL_FACE);
  glCullFace(GL_BACK);

  glEnable(GL_DEPTH_TEST);            // Разрешить тест глубины
  glDepthFunc(GL_LEQUAL);            // Тип теста глубины

  glClearColor(117.0f / 255.0f, 187.0f / 255.0f, 253.0f / 255.0f, 1.0f);

  auto bs = Resources::Get().LoadShader("shaders/basic.glsl");
  bs->Use();
  bs->SetUniform(TEXTURE_SLOT_0, "atlas");
  bs->SetUniform(TEXTURE_SLOT_2, "shadowmap");
  bs->SetUniform(TEXTURE_SLOT_4, "rgbtable");
  bs->SetUniform(glm::vec3(100), "lightpos");
  Resources::Get().LoadTexture("data\\rgbtable.png", false, true, TEXTURE_DIM_3, {16,16,16});
  Resources::Get().GetTexture("data\\rgbtable.png")->Set(TEXTURE_SLOT_4);

  SectorTessellator::Init();

  auto bs2 = Resources::Get().LoadShader("shaders/shadow.glsl");
  bs2->Use();
  bs2->SetUniform(TEXTURE_SLOT_0, "atlas");

  auto bs3 = Resources::Get().LoadShader("shaders/clouds.glsl");
  bs3->Use();
  bs3->SetUniform(TEXTURE_SLOT_4, "noisetex");

  Resources::Get().LoadMesh("data/models/selection.obj");
  Resources::Get().LoadMesh("data/models/test.obj");
  Resources::Get().LoadMesh("data/models/pipe_f.obj");
  Resources::Get().LoadMesh("data/models/pipe_b.obj");
  Resources::Get().LoadMesh("data/models/pipe_l.obj");
  Resources::Get().LoadMesh("data/models/pipe_r.obj");
  Resources::Get().LoadMesh("data/models/pipe_u.obj");
  Resources::Get().LoadMesh("data/models/pipe_d.obj");

  Resources::Get().LoadTexture("data\\noisetex.png");
  Resources::Get().GetTexture("data\\noisetex.png")->Set(TEXTURE_SLOT_3);

  auto par = Resources::Get().LoadShader("shaders/particles.glsl");
  par->Use();
  par->SetUniform(TEXTURE_SLOT_0, "atlas");
  par->SetUniform(TEXTURE_SLOT_2, "shadowmap");
  par->SetUniform(TEXTURE_SLOT_4, "rgbtable");

  auto colo = Resources::Get().LoadShader("shaders/color.glsl");
  colo->Use();
  colo->SetUniform(TEXTURE_SLOT_0, "atlas");

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
  mModels.push_back(std::move(std::make_unique<Model>()));

  auto &model = *mModels.back();

  PMesh<VertexVTN> m = std::make_shared<TemplateMesh<VertexVTN>>(*Resources::Get().GetMesh(mesh));
  auto tex_tuple = TextureManager::Get().GetTexture(texture);
  const auto &uv4 = std::get<1>(tex_tuple);
  for (int i = 0; i < m->SizeVertex(); i++)
  {
	  m->Vertex(i).texture.x = glm::mix(uv4.x, uv4.z, m->Vertex(i).texture.x);
	  m->Vertex(i).texture.y = glm::mix(uv4.y, uv4.w, m->Vertex(i).texture.y);
  }
  model.mTexture = std::get<0>(tex_tuple);
  model.mShader = Resources::Get().GetShader(shader);
  model.mMeshes = m;

  model.Compile();
  model.Release();

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

  model.SetModel(matrix);
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

    const auto &aabb = model.GetAABB();
    if (!camera.BoxWithinFrustum(model.GetModel() * std::get<0>(aabb), model.GetModel() * std::get<1>(aabb)))
    {
      continue;
    }

    model.GetTexture()->Set(TEXTURE_SLOT_0);

    auto &shader = model.GetShader();
    shader->Use();

    shader->SetUniform(camera.GetViewProject() * model.GetModel(), "transform_VP");

    model.GetMesh()->Draw();
  }
}

