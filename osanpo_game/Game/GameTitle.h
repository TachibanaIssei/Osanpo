#pragma once
#include "sound/SoundSource.h"

class GameTitle : public IGameObject
{
public:
	GameTitle();
	~GameTitle();
	void Update();
	void setumeigamen();
	void Render(RenderContext& rc);

	SpriteRender spriteRender;
	SpriteRender setumeiRender1;
	SpriteRender setumeiRender2;
	SoundSource* titleBGM = nullptr;

private:
	int hyouzi = 0; //説明画面を表示するフラグ
	int g_sw = 0;   //説明画面のページ切り替えのフラグ
	int gamestart_flag = 0; //ゲームを開始できるフラグ
};

