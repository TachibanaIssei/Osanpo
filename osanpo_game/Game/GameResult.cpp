#include "stdafx.h"
#include "GameResult.h"
#include "GameTitle.h"
#include "sound/SoundSource.h"
#include "sound/SoundEngine.h"

GameResult::GameResult()
{
	spriteRender.Init("Assets/sprite/result.dds", 1920.0f, 1080.0f);

	//�Q�[������BGM��ǂݍ��ށB
	g_soundEngine->ResistWaveFileBank(5, "Assets/sound/resultBGM.wav");
	//�Q�[������BGM���Đ�����B
	resultBGM = NewGO<SoundSource>(0);
	resultBGM->Init(5);
	resultBGM->SetVolume(0.4f);
	resultBGM->Play(true);

	//�{�^��se
	g_soundEngine->ResistWaveFileBank(6, "Assets/sound/buttonSE.wav");
}

GameResult::~GameResult()
{
	DeleteGO(resultBGM);
}

void GameResult::Update()
{
	Time();
	Coin();
	Score();

	if (g_pad[0]->IsTrigger(enButtonA))
	{
		//se�̍Đ�
		SoundSource* se = NewGO<SoundSource>(0);
		se->Init(6);
		se->SetVolume(2.0f);
		se->Play(false);

		NewGO<GameTitle>(0, "gametitle");
		DeleteGO(this);
	}
}

void GameResult::Time()
{
	//���^�C���̃e�L�X�g�ݒ�
	wchar_t time_m[255];
	swprintf_s(time_m, 255, L"%d", int(timer_m) / 60);
	//�b�^�C���̃e�L�X�g�ݒ�
	wchar_t time_s[255];
	swprintf_s(time_s, 255, L"%d", int(timer_s));

	//���Ԃ̕\��
	timerM.SetText(time_m);
	timerM.SetPosition(Vector3(250.0f, 135.0f, 0.0f));
	timerM.SetScale(2.5f);
	timerM.SetColor(g_vec4White);

	timerS.SetText(time_s);
	
	//�b�����ꌅ�̎��ƂQ���̎��Ƀ|�W�V������ς���
	if (int(timer_s) / 10 == 0)
	{
		timerS.SetPosition(Vector3(475.0f, 135.0f, 0.0f));
	}
	else {
		timerS.SetPosition(Vector3(450.0f, 135.0f, 0.0f));
	}

	timerS.SetScale(2.5f);
	timerS.SetColor(g_vec4White);
}

void GameResult::Coin()
{
	wchar_t coin[255];
	swprintf_s(coin, 255, L"%d", coin_count);
	coinFont.SetText(coin);

	if (coin_count / 10 == 0)
	{
		coinFont.SetPosition(Vector3(475.0f, 265.0f, 0.0f));
	}
	else {
		coinFont.SetPosition(Vector3(445.0f, 265.0f, 0.0f));
	}

	if (coin_count / 100 >= 1)
	{
		coinFont.SetPosition(Vector3(395.0f, 265.0f, 0.0f));
	}
	coinFont.SetScale(2.5f);
	coinFont.SetColor(g_vec4White);
}

void GameResult::Score()
{
	wchar_t score[255];
	int score_in = 0;

	//�R�C���P����100�X�R�A�l��
	score_in += (coin_count * 100);
	
	//�������Ԃ�3���̎���1000�X�R�A�l��
	//�P�����Ƃ�100�X�R�A�l��
	if ((int)timer_m >= 180)
	{
		score_in += 1000;
	}
	score_in += (((int)timer_m / 60) * 100);

	//�������Ԉ�b���Ƃ�2�X�R�A�l��
	score_in += ((int)timer_s * 2);

	swprintf_s(score, 255, L"%d",score_in);
	scoreFont.SetText(score);
	scoreFont.SetPosition(Vector3(350.0f, -20.0f, 0.0f));
	scoreFont.SetScale(2.5f);
	scoreFont.SetColor(g_vec4White);
}

void GameResult::Render(RenderContext& rc)
{
	spriteRender.Draw(rc);
	timerM.Draw(rc);
	timerS.Draw(rc);
	coinFont.Draw(rc);
	scoreFont.Draw(rc);
}
