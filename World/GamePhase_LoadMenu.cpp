#include "GamePhase_LoadMenu.h"
#include <boost/filesystem/convenience.hpp>
#include "imgui/imgui.h"
#include "gui\imgui_impl_glfw_gl3.h"
#include "Render\TextureManager.h"

GamePhase_LoadMenu::GamePhase_LoadMenu()
{
	boost::filesystem::path targetDir("Save");
	boost::filesystem::recursive_directory_iterator iter(targetDir);

	for (const boost::filesystem::path &file : iter) {
		if (is_regular_file(file) && extension(file) == ".world")
		{
			saves.push_back(file.string());
		}
	}

	savec_c = std::vector<const char *>(saves.size());
	for (size_t i = 0; i < saves.size(); ++i)
	{
		savec_c[i] = saves[i].data();
	}
}

GamePhase_LoadMenu::~GamePhase_LoadMenu()
{
}

void GamePhase_LoadMenu::Draw(float gt)
{
	glClearColor(0, 0, 0, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	static std::unique_ptr<GamePhase> game;

	ImGui_ImplGlfwGL3_NewFrame();
	{
		ImGui::SetNextWindowSize({ Game::GetWindow()->GetSize().x / 2.f, Game::GetWindow()->GetSize().y / 2.f });
		ImGui::SetNextWindowPosCenter();

		ImGui::Begin("Load game", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar);
		static int current;
		if (ImGui::ListBox("Saves", &current, savec_c.data(), saves.size(), 10))
		{
			Settings::Get().save_file = savec_c[current];

			static boost::thread game_load([&]()
			{
				game = std::make_unique<GamePhase_Game>();
			});
		}

		if (ImGui::Button("Back"))
		{
			Game::SetGamePhase(std::make_unique<GamePhase_MainMenu>());
		}

		if (game && game->IsInitialized())
		{
			static_cast<GamePhase_Game*>(game.get())->generateShadowFBO();
			TextureManager::Get().Compile();
			Game::SetGamePhase(std::move(game));
		}

		Game::DrawLoading();
		ImGui::End();
	}
	ImGui::Render();
}

void GamePhase_LoadMenu::Update(float gt)
{
}