#pragma once
#include "sound/SoundSource.h"

class GameResult : public IGameObject
{
public:
	GameResult();
	~GameResult();
	void Update();
	void Time();
	void Coin();
	void Score();
	void Render(RenderContext& rc);

	SpriteRender spriteRender;
	FontRender timerM;
	FontRender timerS;
	FontRender coinFont;
	FontRender scoreFont;
	SoundSource* resultBGM = nullptr;

	int coin_count = 0;
	float timer_m = 0.0f;
	float timer_s = 0.0f;
};

