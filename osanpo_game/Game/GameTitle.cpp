#include "stdafx.h"
#include "GameTitle.h"
#include "Game.h"
#include "sound/SoundSource.h"
#include "sound/SoundEngine.h"

GameTitle::GameTitle()
{
	spriteRender.Init("Assets/sprite/title.dds", 1920.0f, 1080.0f);
	setumeiRender1.Init("Assets/sprite/setumei01.dds", 1920.0f, 1080.0f);
	setumeiRender2.Init("Assets/sprite/setumei02.dds", 1920.0f, 1080.0f);

	//�Q�[������BGM��ǂݍ��ށB
	g_soundEngine->ResistWaveFileBank(4, "Assets/sound/titleBGM.wav");
	//�Q�[������BGM���Đ�����B
	titleBGM = NewGO<SoundSource>(0);
	titleBGM->Init(4);
	titleBGM->SetVolume(0.4f);
	titleBGM->Play(true);

	//�{�^��se
	g_soundEngine->ResistWaveFileBank(7, "Assets/sound/buttonSE.wav");
	g_soundEngine->ResistWaveFileBank(9, "Assets/sound/cancel.wav");
}

GameTitle::~GameTitle()
{
	DeleteGO(titleBGM);
}

void GameTitle::Update()
{
	//������ʂ̕\��
	setumeigamen();

	//�Q�[���̊J�n
	if (g_pad[0]->IsTrigger(enButtonA) && gamestart_flag == 0)
	{
		//se�̍Đ�
		SoundSource* se = NewGO<SoundSource>(0);
		se->Init(7);
		se->SetVolume(2.0f);
		se->Play(false);

		NewGO<Game>(0, "game");
		DeleteGO(this);
	}

	spriteRender.Update();
}

void GameTitle::setumeigamen()
{
	//X�{�^���������Ɛ�����ʂ��\������A������x�����ƕ\��������
	if (g_pad[0]->IsTrigger(enButtonX))
	{
		if (hyouzi == 0)
		{
			//se�̍Đ�
			SoundSource* se = NewGO<SoundSource>(0);
			se->Init(7);
			se->SetVolume(2.0f);
			se->Play(false);

			g_sw = 0;
			hyouzi = 1;
			gamestart_flag = 1;
		}
		else {
			//se�̍Đ�
			SoundSource* se = NewGO<SoundSource>(0);
			se->Init(9);
			se->SetVolume(2.0f);
			se->Play(false);

			gamestart_flag = 0;
			hyouzi = 0;
		}
	}

	//������ʂ�\�����ɉE�����{�^���������Ǝ��̃y�[�W�ɍs��
	if (hyouzi == 1 && g_pad[0]->IsTrigger(enButtonRight))
	{
		g_sw = 1;
	}
	//������ʕ\�����ɍ������{�^���������ƑO�̃y�[�W�ɍs��
	if (hyouzi == 1 && g_pad[0]->IsTrigger(enButtonLeft))
	{
		g_sw = 0;
	}

	setumeiRender1.Update();
	setumeiRender2.Update();
}

void GameTitle::Render(RenderContext& rc)
{
	spriteRender.Draw(rc);
	
	if (hyouzi == 1)
	{
		if (g_sw == 0)
		{
			setumeiRender1.Draw(rc);
		}
		else
		{
			setumeiRender2.Draw(rc);
		}
	}
}
