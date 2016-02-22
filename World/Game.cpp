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

  TextureManager::Get().LoadDirectory("Textures");
  TextureManager::Get().Compile();

  DB::Get().ReloadDirectory("json/");

  std::atomic<bool> close = false;

  mWorld->GetPlayer()->SetPosition({ 0,0,800 });

  boost::thread th([&close]() {
    while (!close)
    {
      WorldWorker::Get().Process();
      std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
  });

  mSectorLoader = std::make_unique<SectorLoader>(*mWorld, SPos{}, 10);

  mTessellator = std::make_unique<Tessellator>(*mRenderSector);
  mWorld->SetTessellator(mTessellator.get());
  mTessellator->Run();

  boost::thread thread([this, &close]
  {
    auto currTime = glfwGetTime();
    while (!close)
    {
      auto lastTime = currTime;
      currTime = glfwGetTime();
      Update(currTime - lastTime);
      std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
  });

  auto currTime = glfwGetTime();
  while (!mWindow->WindowShouldClose())
  {
    fps.Update();

    auto lastTime = currTime;
    currTime = glfwGetTime();
    Draw(currTime - lastTime);

    mWindow->Update();
	std::this_thread::sleep_for(std::chrono::milliseconds(1));
  }

  close = true;
  mTessellator.reset();
  thread.join();
  th.join();

  return 0;
}


void Game::Update(double dt)
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


void Game::Draw(double dt)
{
  mCamera->SetPos(mWorld->GetPlayer()->GetPosition() + glm::vec3{ 0.0f, 0.0f, 1.7f });
  mCamera->Update();

  GL_CALL(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));     // Очистка экрана

  mRenderSector->Draw(*mCamera);

  ImGui_ImplGlfwGL3_NewFrame();
  {
    static std::array<float, 100> fps_h, fps_m;
    static float sec{}, minsec{};
    sec += dt;
    minsec += dt;
    static int li = 0, lj = 0;

    static std::string mems;
    if (sec >= 1)
    {
      fps_h[li++] = fps.GetCount();
      if (li == 99)
        li = 0;
      sec -= 1;

      //mems = GetMemoryString(memory_used());
    }

    if (minsec >= 0.1f)
    {
      fps_m[lj++] = fps.GetCount();
      if (lj == 99)
        lj = 0;
      minsec -= 0.1;
    }

    ImGui::Text("Perfomance");
    ImGui::SetWindowSize({ 100, 160 });
    ImGui::SetWindowPos({500,0});

    ImGui::LabelText("Mem:", "%s", mems.c_str());
    ImGui::PlotLines(
      "",
      &fps_h[0],
      100,
      li, std::to_string(fps.GetCount()).c_str(), 0, 4000, {100,50});

    ImGui::PlotLines(
      "",
      &fps_m[0],
      100,
      lj, std::to_string(fps.GetCount()).c_str(), 0, 4000, { 100,50 });
  }
  ImGui::Render();

  //if(0)
  //{
  //  ImGui_ImplGlfwGL3_NewFrame();
  //  bool show_test_window = true, show_another_window = true;
  //  {
  //    static float f = 0.0f;
  //    ImGui::Text("Hello, world!");
  //    ImGui::SliderFloat("float", &f, 0.0f, 1.0f);
  //    ImGui::ColorEdit3("clear color", (float*)&glm::vec4(1, 1, 1, 1)[0]);
  //    if (ImGui::Button("Test Window")) show_test_window ^= 1;
  //    if (ImGui::Button("Another Window")) show_another_window ^= 1;
  //    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
  //  }

  //  // 2. Show another simple window, this time using an explicit Begin/End pair
  //  if (show_another_window)
  //  {
  //    ImGui::SetNextWindowSize(ImVec2(200, 100), ImGuiSetCond_FirstUseEver);
  //    ImGui::Begin("Another Window", &show_another_window);
  //    ImGui::Text("Hello");
  //    ImGui::End();
  //  }

  //  // 3. Show the ImGui test window. Most of the sample code is in ImGui::ShowTestWindow()
  //  if (show_test_window)
  //  {
  //    ImGui::SetNextWindowPos(ImVec2(650, 20), ImGuiSetCond_FirstUseEver);
  //    ImGui::ShowTestWindow(&show_test_window);
  //  }

  //  ImGui::Render();
  //}
}

