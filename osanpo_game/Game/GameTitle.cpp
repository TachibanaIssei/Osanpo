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

	//ゲーム中のBGMを読み込む。
	g_soundEngine->ResistWaveFileBank(4, "Assets/sound/titleBGM.wav");
	//ゲーム中のBGMを再生する。
	titleBGM = NewGO<SoundSource>(0);
	titleBGM->Init(4);
	titleBGM->SetVolume(0.4f);
	titleBGM->Play(true);

	//ボタンse
	g_soundEngine->ResistWaveFileBank(7, "Assets/sound/buttonSE.wav");
	g_soundEngine->ResistWaveFileBank(9, "Assets/sound/cancel.wav");
}

GameTitle::~GameTitle()
{
	DeleteGO(titleBGM);
}

void GameTitle::Update()
{
	//説明画面の表示
	setumeigamen();

	//ゲームの開始
	if (g_pad[0]->IsTrigger(enButtonA) && gamestart_flag == 0)
	{
		//seの再生
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
	//Xボタンを押すと説明画面が表示され、もう一度押すと表示を消す
	if (g_pad[0]->IsTrigger(enButtonX))
	{
		if (hyouzi == 0)
		{
			//seの再生
			SoundSource* se = NewGO<SoundSource>(0);
			se->Init(7);
			se->SetVolume(2.0f);
			se->Play(false);

			g_sw = 0;
			hyouzi = 1;
			gamestart_flag = 1;
		}
		else {
			//seの再生
			SoundSource* se = NewGO<SoundSource>(0);
			se->Init(9);
			se->SetVolume(2.0f);
			se->Play(false);

			gamestart_flag = 0;
			hyouzi = 0;
		}
	}

	//説明画面を表示中に右方向ボタンを押すと次のページに行く
	if (hyouzi == 1 && g_pad[0]->IsTrigger(enButtonRight))
	{
		g_sw = 1;
	}
	//説明画面表示中に左方向ボタンを押すと前のページに行く
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
