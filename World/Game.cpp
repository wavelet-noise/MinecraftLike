// ============================================================================
// ==                   Copyright (c) 2015, Smirnov Denis                    ==
// ==                  See license.txt for more information                  ==
// ============================================================================
#include "Game.h"

#include <gl/glew.h>
#include <iostream>
#include <sstream>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <boost/thread/thread.hpp>
#include <boost\circular_buffer.hpp>

#include <fstream>
#include <memory>
#include "Render/OpenGLCall.h"
#include <vector>
#include <thread>
#include <atomic>
#include "tools/Bresenham3D.h"
#include "tools/CoordSystem.h"
#include "Core\MapGen\WorldWorker.h"
#include "Render/TextureManager.h"
#include "Core/DB.h"
#include "Core/SplitBlockTessellator.h"
#include "tools/Log.h"
#include "tools\wset.h"
#include "tools\order_casters.h"

#include "gui\imgui_impl_glfw_gl3.h"
#include "gui\WindowPerfomance.h"
#include "gui\WindowDb.h"
#include "gui\WindowInventory.h"
#include <Render\Resourses.h>
#include <tools\ray.h>

Game::Game()
{
  Window::WindowSystemInitialize();

  mWindow = std::make_unique<Window>(glm::uvec2(600, 600));
  mCamera = std::make_shared<Camera>();
  mWindow->SetCurrentContext();

  mSun = std::make_shared<Camera>();
  mSun->SetShadow();

  Render::Initialize();
  mRender = std::make_unique<Render>();
  mRenderSector = std::make_unique<RenderSector>();

  ImGui_ImplGlfwGL3_Init(mWindow->Get(), true);

  Initialized = true;

  //GL_CALL(glViewport(0, 0, REGISTRY_GRAPHIC.GetWindow().GetSize().x, REGISTRY_GRAPHIC.GetWindow().GetSize().y)); 

  mWorld = std::make_unique<World>();
  WindowInventory::Get().w = mWorld.get();
  mKeyBinder = std::make_unique<KeyBinder>(mWindow->GetKeyboard(), mWindow->GetMouse());

  mKeyBinder->SetCallback([world = mWorld.get()](std::unique_ptr<GameEvent> event)
  {
    world->PushEvent(std::move(event));
  });

  generateShadowFBO();
}

Game::~Game()
{
  mKeyBinder.reset();
  mWorld.reset();
  mRenderSector.reset();
  mRender.reset();
  mWindow.reset();
  Window::WindowSystemFinally();
}


int Game::Run()
{
  if (!Initialized)
  {
    system("pause");
    return -1;
  }

  mCamera->Resize(mWindow->GetSize());
  mWindow->SetResizeCallback([&](int x, int y) {
    if (y == 0)
      y = 1;
    mCamera->Resize({ x,y });
    mSun->Resize({ x,y });
    int width, height;
    glfwGetFramebufferSize(mWindow->Get(), &width, &height);
    glViewport(0, 0, width, height);
    generateShadowFBO();
  });

  TextureManager::Get().LoadDirectory("data\\textures\\");
  TextureManager::Get().Compile();

  DB::Get().ReloadDirectory("data\\json\\");

  mWorld->GetPlayer()->SetPosition({ 0,0,50 });

  boost::thread gen_thread([]() {
    while (true)
    {
      WorldWorker::Get().Process();
      boost::this_thread::sleep_for(boost::chrono::milliseconds(1));
    }
  });

  mSectorLoader = std::make_unique<SectorLoader>(*mWorld, SPos{}, 7);

  mTessellator = std::make_unique<Tessellator>(*mRenderSector);
  mWorld->SetTessellator(mTessellator.get());
  mTessellator->Run();

  //   boost::thread update_thread([this]
  //   {
  //     auto currTime = static_cast<float>(glfwGetTime());
  //     while (true)
  //     {
  //       auto lastTime = currTime;
  //       currTime = static_cast<float>(glfwGetTime());
  //       Update(currTime - lastTime);
  //       boost::this_thread::sleep_for(boost::chrono::milliseconds(1));
  //     }
  //   });

  auto currTime = static_cast<float>(glfwGetTime());
  while (!mWindow->WindowShouldClose())
  {
    fps.Update();

    auto lastTime = currTime;
    currTime = static_cast<float>(glfwGetTime());

    Update(currTime - lastTime);
    Draw(currTime - lastTime);

    mWindow->Update();
    //std::this_thread::sleep_for(std::chrono::milliseconds(1)); ?!
  }

  mTessellator.reset();

  //update_thread.interrupt();
  gen_thread.interrupt();

  //LOG(trace) << "update joining";
  //update_thread.join();

  LOG(trace) << "generate joining";
  gen_thread.join();

  LOG(trace) << "quit";

  return 0;
}


void Game::Update(float dt)
{
  mKeyBinder->Update();

  if (mWindow->GetKeyboard().IsKeyPress(GLFW_KEY_TAB))
  {
    mWindow->GetMouse().SetCentring(!mWindow->GetMouse().GetCentring());
  }

  if (mWindow->GetKeyboard().IsKeyPress(GLFW_KEY_F5))
  {
    auto p = mCamera->GetPos();
    mWorld->GetPlayer()->SetPosition(p + glm::vec3(300));
  }

  SPos secPos = cs::WtoS(mWorld->GetPlayer()->GetPosition());
  mSectorLoader->SetPos(secPos);

  mWorld->Update(static_cast<float>(dt));
}

void Game::generateShadowFBO()
{
  int shadowMapWidth = mWindow->GetSize().x;
  int shadowMapHeight = mWindow->GetSize().y;

  depthTextureId = std::make_shared<Texture>();
  depthTextureId->DepthTexture({ shadowMapWidth, shadowMapHeight });

  GLenum FBOstatus;

  if (fboId)
    glDeleteFramebuffers(1, &fboId);

  glGenFramebuffers(1, &fboId);
  glBindFramebuffer(GL_FRAMEBUFFER, fboId);

  glDrawBuffer(GL_NONE);
  glReadBuffer(GL_NONE);

  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthTextureId->GetId(), 0);

  FBOstatus = glCheckFramebufferStatus(GL_FRAMEBUFFER);
  if (FBOstatus != GL_FRAMEBUFFER_COMPLETE)
    LOG(fatal) << "framebuffer incomplete";

  glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Game::Draw(float dt)
{
  mCamera->SetPos(mWorld->GetPlayer()->GetPosition() + glm::vec3{ 0.0f, 0.0f, 1.7f });
  mCamera->SetRot(mWorld->GetPlayer()->GetRot());
  mCamera->Update();

  mSun->SetPos(mCamera->GetPos() + glm::vec3{ 300 });
  mSun->LookAt(mCamera->GetPos());
  mSun->Update();

  GL_CALL(glBindFramebuffer(GL_FRAMEBUFFER, fboId));
  GL_CALL(glClear(GL_DEPTH_BUFFER_BIT));
  GL_CALL(glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE));
  GL_CALL(glCullFace(GL_FRONT));
  mRenderSector->ShadowDraw(*mSun, Resourses::Get().GetShader("shaders/shadow.glsl"));

  GL_CALL(glBindFramebuffer(GL_FRAMEBUFFER, 0));
  GL_CALL(glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE));
  GL_CALL(glCullFace(GL_BACK));
  GL_CALL(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
  depthTextureId->Set(TEXTURE_SLOT_1);
  mRenderSector->Draw(*mCamera, *mSun);
  mRender->Draw(*mCamera);

  auto ray = mCamera->GetRay(mWindow->GetMouse().GetPos());
  auto cells = Bresenham3D(ray.origin(), ray.origin() + ray.dir()*100.f);

  static std::vector<size_t> models = [&]()->auto {
    std::vector<size_t> t;
    for (int i = 0; i < 10; i++)
      t.push_back(mRender->AddModel("data/models/selection.obj", "dirt", "shaders/basic.glsl"));
    return t;
  }();

  int iii = 0;
  static std::unordered_map<glm::ivec3, PGameObject> opened_w;
  for (const auto &c : cells)
  {
    if (auto &b = mWorld->GetBlock(c))
    {
      mRender->SetModelMatrix(models[iii], glm::translate(glm::mat4(1), glm::vec3(c)));
      if (!ImGui::IsAnyItemHovered())
      {
        if (ImGui::IsMouseDown(0))
        {
          mWorld->SetBlock(c, nullptr);
        }
        if (ImGui::IsMouseDown(1)) {
          if (auto b = mWorld->GetBlock(c))
          {
            opened_w[c] = b;
          }
        }
      }
      break;
      iii++;
      if(iii >= 10)
        break;
    }
  }

  WindowPerfomance &wp = WindowPerfomance::Get();
  WindowInventory &winv = WindowInventory::Get();
  WindowDb &wdb = WindowDb::Get();

  ImGui_ImplGlfwGL3_NewFrame();
  wp.DtUpdate(dt, fps.GetCount());
  wp.Draw();
  wdb.Draw();
  winv.Draw();
  for (auto &w : opened_w)
  {
    std::get<1>(w)->DrawGui(InteractParams{ mWorld.get(), std::get<0>(w), dt });
  }
  ImGui::Render();
}

