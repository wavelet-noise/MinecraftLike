#pragma once
#include "Game.h"

class GamePhase_LoadMenu : public GamePhase
{
public:
	GamePhase_LoadMenu();
	~GamePhase_LoadMenu();
	void Draw(float gt) override;
	void Update(float gt) override;

	std::vector<std::string> saves;
	std::vector<const char *> savec_c;
};