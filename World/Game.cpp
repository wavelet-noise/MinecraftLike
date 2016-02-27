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

Game::Game()
{
  Window::WindowSystemInitialize();

  mWindow = std::make_unique<Window>(glm::uvec2(600, 600));
  mCamera = std::make_unique<Camera>();
  mWindow->SetCurrentContext();

  Render::Initialize();
  mRender = std::make_unique<Render>();
  mRenderSector = std::make_unique<RenderSector>();

  ImGui_ImplGlfwGL3_Init(mWindow->Get(), true);

  Initialized = true;

  //GL_CALL(glViewport(0, 0, REGISTRY_GRAPHIC.GetWindow().GetSize().x, REGISTRY_GRAPHIC.GetWindow().GetSize().y)); 

  mWorld = std::make_unique<World>();
}

Game::~Game()
{
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
    mCamera->Resize({x,y});
    int width, height;
    glfwGetFramebufferSize(mWindow->Get(), &width, &height);
    glViewport(0, 0, width, height);
  });

  TextureManager::Get().LoadDirectory("Textures");
  TextureManager::Get().Compile();

  DB::Get().ReloadDirectory("json/");

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

  boost::thread update_thread([this]
  {
    auto currTime = static_cast<float>(glfwGetTime());
    while (true)
    {
      auto lastTime = currTime;
      currTime = static_cast<float>(glfwGetTime());
      Update(currTime - lastTime);
      boost::this_thread::sleep_for(boost::chrono::milliseconds(1));
    }
  });

  auto currTime = static_cast<float>(glfwGetTime());
  while (!mWindow->WindowShouldClose())
  {
    fps.Update();

    auto lastTime = currTime;
    currTime = static_cast<float>(glfwGetTime());
    Draw(currTime - lastTime);

    mWindow->Update();
	  //std::this_thread::sleep_for(std::chrono::milliseconds(1)); ?!
  }

  mTessellator.reset();

  update_thread.interrupt();
  gen_thread.interrupt();

  LOG(trace) << "update joining";
  update_thread.join();

  LOG(trace) << "generate joining";
  gen_thread.join();

  LOG(trace) << "quit";

  return 0;
}


void Game::Update(float dt)
{
  float speedRot = static_cast<float>(3.0 * dt);

  if (mWindow->GetKeyboard().IsKeyDown(GLFW_KEY_LEFT))
  {
    mWorld->GetPlayer()->Rotate({ 0.0f, 0.0f, -speedRot });
    mCamera->Rotate({ 0.0f, 0.0f, -speedRot });
  }
  if (mWindow->GetKeyboard().IsKeyDown(GLFW_KEY_RIGHT))
  {
    mWorld->GetPlayer()->Rotate({ 0.0f, 0.0f, speedRot });
    mCamera->Rotate({ 0.0f, 0.0f, speedRot });
  }
  if (mWindow->GetKeyboard().IsKeyDown(GLFW_KEY_DOWN))
  {
    mWorld->GetPlayer()->Rotate({ speedRot, 0.0f, 0.0f });
    mCamera->Rotate({ speedRot, 0.0f, 0.0f });
  }
  if (mWindow->GetKeyboard().IsKeyDown(GLFW_KEY_UP))
  {
    mWorld->GetPlayer()->Rotate({ -speedRot, 0.0f, 0.0f });
    mCamera->Rotate({ -speedRot, 0.0f, 0.0f });
  }

  if (mWindow->GetMouse().GetCentring())
  {
    auto moved = mWindow->GetMouse().GetMoved();
    moved *= 0.001f;
    mWorld->GetPlayer()->Rotate(glm::vec3(moved.y, 0.0f, moved.x));
    mCamera->Rotate(glm::vec3(moved.y, 0.0f, moved.x));
  }

  static float k = 1.0f;
  if (mWindow->GetKeyboard().IsKeyPress(GLFW_KEY_9))
  {
    k += 1.0f;
  }
  if (mWindow->GetKeyboard().IsKeyPress(GLFW_KEY_0))
  {
    k -= 1.0f;
  }
  if (k < 1.0f)
  {
    k = 1.0f;
  }
  float speedMov = static_cast<float>(15.0 * dt) * k;

  if (mWindow->GetKeyboard().IsKeyDown(GLFW_KEY_D))
  {
    mWorld->GetPlayer()->Move({ speedMov, 0.0f, 0.0f });
  }
  if (mWindow->GetKeyboard().IsKeyDown(GLFW_KEY_A))
  {
    mWorld->GetPlayer()->Move({ -speedMov, 0.0f, 0.0f });
  }
  if (mWindow->GetKeyboard().IsKeyDown(GLFW_KEY_W))
  {
    mWorld->GetPlayer()->Move({ 0.0f, speedMov, 0.0f });
  }
  if (mWindow->GetKeyboard().IsKeyDown(GLFW_KEY_S))
  {
    mWorld->GetPlayer()->Move({ 0.0f, -speedMov, 0.0f });
  }
  if (mWindow->GetKeyboard().IsKeyPress(GLFW_KEY_SPACE))
  {
    mWorld->GetPlayer()->Accelerate({ 0.0f, 0.f, 0.03f });
  }
  if (mWindow->GetKeyboard().IsKeyPress(GLFW_KEY_1))
  {
    mWorld->GetPlayer()->SetPosition({ 0.0f, 0.f, 100.0f });
    mWorld->GetPlayer()->SetAcceleration({ 0.0f, 0.f, 0.f });
  }
  if (mWindow->GetKeyboard().IsKeyPress(GLFW_KEY_TAB))
  {
    mWindow->GetMouse().SetCentring(!mWindow->GetMouse().GetCentring());
  }

  if (mWindow->GetKeyboard().IsKeyDown(GLFW_KEY_F5))
  {
    static int i = -20;
    static int j = 0;
    i++;
    if (i > 20)
    {
      j++;
      i = -20;
    }
    mWorld->GetSector(glm::vec3(i, j, 0));
  }

  SPos secPos = cs::WtoS(mWorld->GetPlayer()->GetPosition());
  mSectorLoader->SetPos(secPos);

  mWorld->Update(static_cast<float>(dt));
}


void Game::Draw(float dt)
{
  mCamera->SetPos(mWorld->GetPlayer()->GetPosition() + glm::vec3{ 0.0f, 0.0f, 1.7f });
  mCamera->Update();

  GL_CALL(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));     // Очистка экрана

  mRenderSector->Draw(*mCamera);

  WindowPerfomance &wp = WindowPerfomance::Get();
  static WindowDb wdb;

  ImGui_ImplGlfwGL3_NewFrame();
  wp.DtUpdate(dt, fps.GetCount());
  wp.Draw();
  wdb.Draw();
  ImGui::Render();
}

