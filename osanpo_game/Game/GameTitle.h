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
	int hyouzi = 0; //������ʂ�\������t���O
	int g_sw = 0;   //������ʂ̃y�[�W�؂�ւ��̃t���O
	int gamestart_flag = 0; //�Q�[�����J�n�ł���t���O
};

