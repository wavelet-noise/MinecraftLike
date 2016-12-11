#ifndef GAME_H
#define GAME_H

#include <memory>
#include "Window\Window.h"
#include "Render\Camera.h"
#include "Render\Render.h"
#include "Core\World.h"
#include "Core\SectorLoader.h"
#include "Core\Tessellator.h"

#include "FpsCounter.h"
#include "Render\RenderSector.h"
#include "network\NetworkManager.h"
#include <render\spritebatch.h>
#include <boost/thread/thread.hpp>
#include <boost/filesystem/path.hpp>

class WorldWorker;

class GamePhase
{
public:
	GamePhase() {}
	virtual ~GamePhase() {}
	virtual void Draw(float gt) {}
	virtual void Update(float gt) {}
	virtual bool IsInitialized() { return true; }
};

class GamePhase_Game : public GamePhase
{
public:
	GamePhase_Game();
	void generateShadowFBO();
	~GamePhase_Game();
	void Draw(float gt) override;
	void Update(float gt) override;
	bool IsInitialized() override;

private:
	boost::thread gen_thread;
	std::unique_ptr<SectorLoader> mSectorLoader;

	std::shared_ptr<World> mWorld;
	std::unique_ptr<RenderSector> mRenderSector;
	std::unique_ptr<Tessellator> mTessellator;

	PTexture depthTextureId;
	unsigned fboId = 0;
	bool initialized = false;
};

class GamePhase_MainMenu : public GamePhase
{
public:
	GamePhase_MainMenu();
	~GamePhase_MainMenu();
	void Draw(float gt) override;
	void Update(float gt) override;
};

class Game
{
public:
	Game();
	~Game();
	void GameRun();
	int Run();

	static Window * GetWindow();
	static Render * GetRender();
	static SpriteBatch * GetSpriteBatch();
	static FpsCounter * GetFps();
	static Camera * GetCamera();
	static Camera * GetSun();
	static WorldWorker * GetWorker();
	static void SetWorker(std::unique_ptr<WorldWorker> &&ww);

	static void SetGamePhase(std::unique_ptr<GamePhase> &&gp);
	static void SetLoadPhase(float percent, std::string descr, int phase, int max_phase);
	static void SetLoadPhase(float percent, std::string descr, int phase);
	static void SetLoadPhase(float percent, std::string descr);
	static void SetLoadPhase(float percent);
	static void DrawLoading();

private:
	static float percent;
	static std::string descr;
	static int phase;
	static int max_phase;

	static std::unique_ptr<GamePhase> game_phase;

private:

	bool Initialized = false;

	static FpsCounter fps;

	static std::unique_ptr<Window> mWindow;
	static std::unique_ptr<Render> mRender;

	static std::unique_ptr<WorldWorker> world_worker;

	static std::unique_ptr<SpriteBatch> sb;
};

#endif // GAME_H
