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
  WindowInventory::Get().w = mWorld.get();
  mKeyBinder = std::make_unique<KeyBinder>(mWindow->GetKeyboard(), mWindow->GetMouse());

  mKeyBinder->SetCallback([world=mWorld.get()](std::unique_ptr<GameEvent> event)
  {
    world->PushEvent(std::move(event));
  });
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
  mKeyBinder->Update();

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
  mCamera->SetRot(mWorld->GetPlayer()->GetRot());
  mCamera->Update();

  GL_CALL(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));     // Очистка экрана

  mRenderSector->Draw(*mCamera);

  WindowPerfomance &wp = WindowPerfomance::Get();
  WindowInventory &winv = WindowInventory::Get();
  WindowDb &wdb = WindowDb::Get();

  ImGui_ImplGlfwGL3_NewFrame();
  wp.DtUpdate(dt, fps.GetCount());
  wp.Draw();
  wdb.Draw();
  winv.Draw();
  ImGui::Render();
}

