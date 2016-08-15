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

class Game
{
public:
  Game();
  ~Game();

  int Run();

  void Initialize();

  void Update(float dt);

  void generateShadowFBO();

  void Draw(float dt);

  static World * GetWorld();

private:

  bool Initialized = false;
  FpsCounter fps;

  std::unique_ptr<Window> mWindow;
  std::unique_ptr<Render> mRender;
  std::unique_ptr<RenderSector> mRenderSector;
  std::shared_ptr<Camera> mCamera;
  std::shared_ptr<Camera> mSun;
  static std::unique_ptr<World> mWorld;
  std::unique_ptr<Tessellator> mTessellator;

  std::unique_ptr<SectorLoader> mSectorLoader;
  std::unique_ptr<SpriteBatch> sb;

  PTexture depthTextureId;
  unsigned fboId = 0;
};

#endif // GAME_H
