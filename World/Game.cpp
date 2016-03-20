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
#include <core\Chest.h>

Game::Game()
{
	Window::WindowSystemInitialize();

	mWindow = std::make_unique<Window>(glm::uvec2(1024, 768));
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
		mCamera->Resize({ x, y });
		mSun->Resize({ x, y });
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

	mSectorLoader = std::make_unique<SectorLoader>(*mWorld, SPos{}, 5);

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
	depthTextureId = std::make_shared<Texture>();
	depthTextureId->DepthTexture({ 1024, 1024 });

	GLenum FBOstatus;

	if (fboId)
		glDeleteFramebuffers(1, &fboId);

	fboId = -1;

	glGenFramebuffers(1, &fboId);
	glBindFramebuffer(GL_FRAMEBUFFER, fboId);

	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthTextureId->GetId(), 0);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Game::Draw(float dt)
{
	mCamera->SetPos(mWorld->GetPlayer()->GetPosition() + glm::vec3{ 0.0f, 0.0f, 1.7f });
	mCamera->SetRot(mWorld->GetPlayer()->GetRot());
	mCamera->Update();

	static float phi = 0;
	//phi += dt / 20.f;
	mSun->SetPos(mCamera->GetPos() + glm::vec3{ glm::sin(phi) + glm::cos(phi), 0, -glm::sin(phi) + glm::cos(phi) });
	mSun->LookAt(mCamera->GetPos());
	mSun->Update();

	static bool wire = false;

	if (ImGui::IsKeyPressed(GLFW_KEY_F2, false))
		wire = wire ? (glPolygonMode(GL_FRONT_AND_BACK, GL_LINE), false) :
		(glPolygonMode(GL_FRONT_AND_BACK, GL_FILL), true);

	static int skip = 1;
	skip++;

	//if (skip >= 5)
	{
		GL_CALL(glBindFramebuffer(GL_FRAMEBUFFER, fboId));
		GL_CALL(glViewport(0, 0, depthTextureId->GetSize().x, depthTextureId->GetSize().y));
		GL_CALL(glClear(GL_DEPTH_BUFFER_BIT));
		GL_CALL(glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE));
		GL_CALL(glCullFace(GL_FRONT));
		mRenderSector->ShadowDraw(*mSun, Resourses::Get().GetShader("shaders/shadow.glsl"));
		skip = 0;
	}

	GL_CALL(glBindFramebuffer(GL_FRAMEBUFFER, 0));
	GL_CALL(glViewport(0, 0, mWindow->GetFbSize().x, mWindow->GetFbSize().y));
	GL_CALL(glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE));
	GL_CALL(glCullFace(GL_BACK));
	auto col = glm::mix(glm::vec4(117.0f / 255.0f, 187.0f / 255.0f, 253.0f / 255.0f, 1.0f), glm::vec4(0, 0.1, 0.2, 1), (glm::sin(phi) - glm::cos(phi) + 1) / 2.f);
	GL_CALL(glClearColor(col.x, col.y, col.z, col.w));
	GL_CALL(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
	depthTextureId->Set(TEXTURE_SLOT_2);
	mRenderSector->Draw(*mCamera, *mSun);
	mRender->Draw(*mCamera);

	auto ray = mCamera->GetRay(mWindow->GetMouse().GetPos());
	std::tuple<glm::ivec3, glm::vec3> selection_pos; // pos, normal

	static size_t select_model = mRender->AddModel("data/models/selection.obj", "selection", "shaders/basic.glsl");
	static size_t break_model = mRender->AddModel("data/models/selection.obj", "break", "shaders/basic.glsl");
	static float break_progress = 0;

	static std::unordered_map<glm::ivec3, PGameObject> opened_w;

	auto sel = mWorld->GetPlayer()->GetFromFullName<Chest>("Chest")->PopSelected();
	if (sel.obj && sel.obj->IsPlacable())
		selection_pos = PickPrefirst(ray.origin(), ray.dir(), 100.f, [&](const glm::ivec3 &pos)->bool {
		return mWorld->GetBlock(pos).get();
	});
	else
		selection_pos = PickFirst(ray.origin(), ray.dir(), 100.f, [&](const glm::ivec3 &pos)->bool {
		return mWorld->GetBlock(pos).get();
	});

	mRender->SetModelMatrix(select_model, glm::translate(glm::mat4(1), glm::vec3(std::get<0>(selection_pos))));
	if (!ImGui::IsAnyItemHovered())
	{
		mRender->SetModelMatrix(break_model, glm::translate(glm::mat4(1), {9999,0,0}));
		if (ImGui::IsMouseDown(0))
		{
			if (sel.obj && sel.obj->IsPlacable())
				mWorld->SetBlock(std::get<0>(selection_pos), DB::Get().Create(sel.obj->GetId()));
			else
			{
				break_progress += dt;
				mRender->SetModelMatrix(break_model, glm::translate(glm::mat4(1), glm::vec3(std::get<0>(selection_pos))));
				if (break_progress >= 0.5)
				{
					break_progress = 0;
					mWorld->SetBlock(std::get<0>(selection_pos), nullptr);
				}
			}
		}
		if (ImGui::IsMouseDown(1)) {
			if (auto b = mWorld->GetBlock(std::get<0>(selection_pos)))
			{
				b->Interact(InteractParams{ mWorld.get(), std::get<0>(selection_pos), dt });
				opened_w[std::get<0>(selection_pos)] = b;
			}
		}
	}
	mWorld->GetPlayer()->GetFromFullName<Chest>("Chest")->PushSelected(std::move(sel));

	WindowPerfomance &wp = WindowPerfomance::Get();
	WindowInventory &winv = WindowInventory::Get();
	WindowDb &wdb = WindowDb::Get();

	ImGui_ImplGlfwGL3_NewFrame();
	wp.DtUpdate(dt, fps.GetCount(), mRenderSector->ApproxDc(), mWorld->GetActiveCount());
	wp.Draw();
	wdb.Draw();
	winv.Draw();
	for (auto &w : opened_w)
	{
		ImGui::Begin((boost::format("Block UI (%1%, %2%, %3%)") % std::get<0>(w).x % std::get<0>(w).y % std::get<0>(w).x).str().c_str());
		std::get<1>(w)->DrawGui();
		ImGui::End();
	}
	ImGui::Render();
}

