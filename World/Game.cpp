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
#include "gui\WS.h"
#include <gui\WindowInventory.h>
#include <gui\WindowCraftQueue.h>
#include <gui\WindowPerfomance.h>
#include <Render\Resources.h>
#include <tools\ray.h>
#include <core\Chest.h>
#include <core\Tool.h>
#include <Render\ParticleSystem.h>
#include <core\EventBus.h>
#include <core\OrderBus.h>

#include <Core\Ore.h>

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
	WindowCraftQueue::Get().w = mWorld.get();
	WindowDb::Get().w = mWorld.get();

	generateShadowFBO();
}

Game::~Game()
{
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
	mCamera->SetPos({ 0, 0, 20 });
	mCamera->LookAt({ 32, 32, 0 });
	mWindow->SetResizeCallback([&](int x, int y) {
		if (y < 1)
			y = 1;
		if (x < 1)
			x = 1;
		mCamera->Resize({ x, y });
		mSun->Resize({ x, y });
	});

	TextureManager::Get().LoadDirectory("data\\textures\\");
	TextureManager::Get().Compile();

	DB::Get().ReloadDirectory("data\\json\\");

	sb = std::make_unique<SpriteBatch>();

	boost::thread gen_thread([]() {
		while (true)
		{
			WorldWorker::Get().Process();
			boost::this_thread::sleep_for(boost::chrono::milliseconds(1));
		}
	});

	mSectorLoader = std::make_unique<SectorLoader>(*mWorld, SPos{}, 5);

	mTessellator = std::make_unique<Tessellator>(*mRenderSector);
	mTessellator->SayCamera(mCamera);
	mWorld->SetTessellator(mTessellator.get());
	mTessellator->Run();

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

	gen_thread.interrupt();

	LOG(trace) << "generate joining";
	gen_thread.join();

	LOG(trace) << "quit";

	return 0;
}


void Game::Update(float dt)
{
	SPos secPos = { 0,0,0 };
	mSectorLoader->SetPos(secPos);

	if (!ImGui::IsAnyItemHovered() && ImGui::IsMouseDragging(1))
	{
		auto del = glm::vec3(ImGui::GetMouseDragDelta(1).y / mWindow->GetSize().y, ImGui::GetMouseDragDelta(1).x / mWindow->GetSize().x, 0);
		del = mCamera->GetDirection() * del;
		del.z = 0;
		mCamera->SetPos(mCamera->GetPos() - del);
	}

	for (const auto &e : EventBus::Get().ListenChannel<EventSectorReady>())
	{
		const auto &esec = std::static_pointer_cast<EventSectorReady>(e);
		if (esec->sec->GetPos() == SPos(0, 0, 0))
		{
			auto c = DB::Get().Create("caracter");
			c->GetAgent<Chest>()->Push(DB::Get().Create("nutrition"), 100);
			controlled.push_back(mWorld->Spawn({ 0, 0, 0 }, c));
			controlled.push_back(mWorld->Spawn({ 0, 0, 0 }, DB::Get().Create("caracter")));
			controlled.push_back(mWorld->Spawn({ 0, 0, 0 }, DB::Get().Create("caracter")));
			controlled.push_back(mWorld->Spawn({ 0, 0, 0 }, DB::Get().Create("caracter")));
			controlled.push_back(mWorld->Spawn({ 0, 0, 0 }, DB::Get().Create("caracter")));
			controlled.push_back(mWorld->Spawn({ 0, 0, 0 }, DB::Get().Create("caracter")));
			controlled.push_back(mWorld->Spawn({ 0, 0, 0 }, DB::Get().Create("caracter")));
			controlled.push_back(mWorld->Spawn({ 0, 0, 0 }, DB::Get().Create("caracter")));
			controlled.push_back(mWorld->Spawn({ 0, 0, 0 }, DB::Get().Create("caracter")));
			controlled.push_back(mWorld->Spawn({ 0, 0, 0 }, DB::Get().Create("caracter")));
			controlled.push_back(mWorld->Spawn({ 0, 0, 0 }, DB::Get().Create("caracter")));
			controlled.push_back(mWorld->Spawn({ 0, 0, 0 }, DB::Get().Create("caracter")));
			controlled.push_back(mWorld->Spawn({ 0, 0, 0 }, DB::Get().Create("caracter")));
			controlled.push_back(mWorld->Spawn({ 0, 0, 0 }, DB::Get().Create("caracter")));
		}
	}

	mWorld->Update(static_cast<float>(dt));
	OrderBus::Get().Update(dt);
	EventBus::Get().Update();
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
	float drawtime = glfwGetTime();
	//mCamera->SetPos(mWorld->GetPlayer()->GetPosition() + glm::vec3{ 0.0f, 0.0f, 1.7f });
	//mCamera->SetRot(mWorld->GetPlayer()->GetRot());
	mCamera->Update();

	static float phi = 0;
	//phi += dt / 20.f;
	mSun->SetPos(mCamera->GetPos() + glm::vec3{ glm::sin(phi) + glm::cos(phi), 0, -glm::sin(phi) + glm::cos(phi) });
	mSun->LookAt(mCamera->GetPos());
	mSun->Update();

	static bool wire = true;

	if (ImGui::IsKeyPressed(GLFW_KEY_F2, false))
		wire = wire ? (glPolygonMode(GL_FRONT_AND_BACK, GL_LINE), false) :
		(glPolygonMode(GL_FRONT_AND_BACK, GL_FILL), true);

	static int skip = 1;
	skip++;

	glBindFramebuffer(GL_FRAMEBUFFER, fboId);
	glViewport(0, 0, depthTextureId->GetSize().x, depthTextureId->GetSize().y);
	glClear(GL_DEPTH_BUFFER_BIT);
	glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
	glCullFace(GL_FRONT);
	mRenderSector->ShadowDraw(*mSun, Resources::Get().GetShader("shaders/shadow.glsl"));
	skip = 0;

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, mWindow->GetFbSize().x, mWindow->GetFbSize().y);
	glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
	glCullFace(GL_BACK);
	auto col = glm::mix(glm::vec4(117.0f / 255.0f, 187.0f / 255.0f, 253.0f / 255.0f, 1.0f), glm::vec4(0, 0.1, 0.2, 1), (glm::sin(phi) - glm::cos(phi) + 1) / 2.f);
	glClearColor(col.x, col.y, col.z, col.w);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	depthTextureId->Set(TEXTURE_SLOT_2);
	mRenderSector->Draw(*mCamera, *mSun);

	mRender->Draw(*mCamera);

	ParticleSystem::Get().Draw(*mCamera);
	ParticleSystem::Get().Update(dt);

	auto ray = mCamera->GetRay(ImGui::GetMousePos());
	std::tuple<glm::ivec3, glm::vec3> selection_pos; // pos, normal

	static size_t select_model = mRender->AddModel("data/models/selection.obj", "selection", "shaders/basic.glsl");
	static size_t square_model = mRender->AddModel("data/models/selection.obj", "selection", "shaders/basic.glsl");

	int j = 0;
	auto ord = OrderBus::Get().orders.begin();
	for (auto i : OrderBus::Get().orders)
	{
		if (i->GetId() == Order::Idfor<OrderDig>())
		{
			auto od = std::static_pointer_cast<OrderDig>(i);
			auto tex_tuple = TextureManager::Get().GetTexture("selection_y");
			auto tex4 = std::get<1>(tex_tuple);
			glm::vec2 uv1 = { tex4.x, tex4.y };
			glm::vec2 uv2 = { tex4.z, tex4.w };

			sb->AddCube(od->pos + glm::vec3(0, 0, 1.05), od->pos + glm::vec3(1, 1, 1.05), uv1, uv2, std::get<0>(tex_tuple));
		}
	}

	static std::unordered_map<glm::ivec3, PGameObject> opened_w;

	if (WindowTools::Get().selected != SelectedOrder::DIG_SQUARE || WindowTools::Get().selected != SelectedOrder::PLACE_BLOCK)
	{
		selection_pos = PickFirst(ray.origin(), ray.dir(), 100.f, [&](const glm::ivec3 &pos)->bool {
			return pos.z == mWorld->GetSlise();
		});
	}
	else
		selection_pos = PickFirst(ray.origin(), ray.dir(), 100.f, [&](const glm::ivec3 &pos)->bool {
		return mWorld->GetBlock(pos).get();
	});

	mRender->SetModelMatrix(select_model, glm::translate(glm::mat4(1), glm::vec3(std::get<0>(selection_pos))));
	if (!ImGui::IsPosHoveringAnyWindow(ImGui::GetMousePos()))
	{
		static glm::vec3 min, max;
		static int minmaxstate = 0;
		if (WindowTools::Get().selected != SelectedOrder::DIG_SQUARE)
		{
			min = max = glm::vec3{ std::numeric_limits<float>().max() };
			minmaxstate = 0;
		}

		if (ImGui::IsMouseDown(0))
		{
			switch (WindowTools::Get().selected)
			{
			case SelectedOrder::DIG_CIRCLE:
			case SelectedOrder::DIG_SQUARE:
				switch (minmaxstate)
				{
				case 0:
					min = std::get<0>(selection_pos);
					minmaxstate = 1;
					break;
				}
				break;
			case SelectedOrder::PLACE_BLOCK:
				OrderBus::Get().IssueOrder(std::make_shared<OrderPlace>(std::get<0>(selection_pos), DB::Get().Create(WindowDb::Get().GetSelectedId())));
				break;
			}
		}

		if (minmaxstate == 1)
		{
			if (!ImGui::IsMouseDown(0))
			{

				max = std::get<0>(selection_pos);

				glm::ivec3 s1 = { glm::min(min.x, max.x), glm::min(min.y, max.y), glm::min(min.z, max.z) };
				glm::ivec3 s2 = { glm::max(min.x, max.x), glm::max(min.y, max.y), glm::max(min.z, max.z) };

				for (int i = s1.x; i <= s2.x; i++)
					for (int j = s1.y; j <= s2.y; j++)
						for (int k = s1.z; k <= s2.z; k++)
						{
							if (WindowTools::Get().selected == SelectedOrder::DIG_SQUARE ||
								(WindowTools::Get().selected == SelectedOrder::DIG_CIRCLE && glm::distance(glm::vec3(i, j, k), glm::vec3(s1 + s2) / 2.f) <= glm::distance(glm::vec3(s1), glm::vec3(s2)) / 2.f))
								if (auto b = mWorld->GetBlock(glm::vec3{ i,j,k }))
								{
									if (Settings::Get().dig_ores || !b->HasAgent<Ore>())
										OrderBus::Get().IssueOrder(std::make_shared<OrderDig>(glm::vec3{ i,j,k }));
								}
						}

				minmaxstate = 0;
			}
			else
			{
				max = std::get<0>(selection_pos);

				glm::ivec3 s1 = { glm::min(min.x, max.x), glm::min(min.y, max.y), glm::min(min.z, max.z) };
				glm::ivec3 s2 = { glm::max(min.x, max.x), glm::max(min.y, max.y), glm::max(min.z, max.z) };

				for (int i = s1.x; i <= s2.x; i++)
					for (int j = s1.y; j <= s2.y; j++)
						for (int k = s1.z; k <= s2.z; k++)
						{
							if (WindowTools::Get().selected == SelectedOrder::DIG_SQUARE ||
								(WindowTools::Get().selected == SelectedOrder::DIG_CIRCLE && glm::distance(glm::vec3(i, j, k), glm::vec3(s1 + s2) / 2.f) <= glm::distance(glm::vec3(s1), glm::vec3(s2)) / 2.f))
							{
								auto tex_tuple = TextureManager::Get().GetTexture("selection_y");
								auto tex4 = std::get<1>(tex_tuple);
								glm::vec2 uv1 = { tex4.x, tex4.y };
								glm::vec2 uv2 = { tex4.z, tex4.w };

								sb->AddCube(glm::vec3(i, j, k + 1.05), glm::vec3(i + 1, j + 1, k + 1.05), uv1, uv2, std::get<0>(tex_tuple), glm::vec4(0, 1, 0, 1));
							}
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

	ImGui_ImplGlfwGL3_NewFrame();
	{
		if (auto b = mWorld->GetBlock(static_cast<WBPos>(std::get<0>(selection_pos))))
		{
			ImGui::SetNextWindowPos({ mWindow->GetSize().x / 2.f - 70, 0 });
			ImGui::SetNextWindowSize({ 140,50 });
			ImGui::Begin("Selected");
			b->DrawGui();
			ImGui::Text("%s", b->GetDescription().c_str());
			ImGui::End();
		}

	}

	WS::Get().Draw(mWindow->GetSize());

	ImGui::Begin("Colony", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
	int i = 0;
	for (auto &c : controlled)
	{
		if (ImGui::TreeNode((std::string("creature_") + std::to_string(i)).c_str()))
		{
			c->DrawGui();
			ImGui::TreePop();
		}
		i++;
	}
	ImGui::End();

	for (auto &w : opened_w)
	{
		ImGui::Begin((boost::format("Block UI (%1%, %2%, %3%)") % std::get<0>(w).x % std::get<0>(w).y % std::get<0>(w).z).str().c_str(), nullptr, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings);

		std::get<1>(w)->DrawGui();
		ImGui::End();
	}

	if (ImGui::IsKeyPressed(GLFW_KEY_ESCAPE))
	{
		opened_w.clear();
	}

	if (!ImGui::IsPosHoveringAnyWindow(ImGui::GetMousePos()))
	{
		int slise = mWorld->GetSlise();
		float prewheel = ImGui::GetIO().MouseWheel;
		mWorld->SetSlise(slise + glm::sign(prewheel));
	}

	ImGui::Render();

	sb->SetCam(mCamera);
	sb->Render();

	WindowPerfomance::Get().DtUpdate(glfwGetTime() - drawtime, fps.GetCount(), mRenderSector->ApproxDc(), mWorld->GetActiveCount());
}

World * Game::GetWorld()
{
	return mWorld.get();
}

std::unique_ptr<World> Game::mWorld;

