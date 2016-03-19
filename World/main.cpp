#include "Game.h"
#include <stdlib.h>
#include <time.h>
#include "tools/Log.h"
#include "Render/Vertex.h"

int main()
{
  srand(static_cast<unsigned int>(time(0)));

  LogInit();

  LOG(info) << "sizeof(StringIntern): " << sizeof(StringIntern);

  Game game;
  return game.Run();
}

