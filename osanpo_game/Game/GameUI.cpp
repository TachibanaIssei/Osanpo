#include "stdafx.h"
#include "GameUI.h"
#include "Player.h"

GameUI::GameUI()
{
	m_player = FindGO<Player>("player");

	//コインUIの画像設定
	coinSprite.Init("Assets/sprite/coinUI.dds", 330.0f, 187.5f);
	coinSprite.SetPosition(Vector3(-850.0f, 440.0f, 0.0f));

	//スターの画像設定
	sterSprite.Init("Assets/sprite/ster.dds", 960.0f, 540.0f);
	sterSprite.SetPosition(Vector3(540.0f, -400.0f, 0.0f));

	//ライフの画像設定
	//1個目
	life01Sprite.Init("Assets/sprite/heart.dds", 150.0f, 125.0f);
	life01Sprite.SetPosition(Vector3(-550.0f, -400.0f, 0.0f));
	//2個目
	life02Sprite.Init("Assets/sprite/heart.dds", 150.0f, 125.0f);
	life02Sprite.SetPosition(Vector3(-700.0f, -400.0f, 0.0f));
	//3個目
	life03Sprite.Init("Assets/sprite/heart.dds", 150.0f, 125.0f);
	life03Sprite.SetPosition(Vector3(-850.0f, -400.0f, 0.0f));
}

GameUI::~GameUI()
{
}

void GameUI::Update()
{
	UI();
	coinSprite.Update();
	sterSprite.Update();
	life01Sprite.Update();
	life02Sprite.Update();
	life03Sprite.Update();
}

void GameUI::UI()
{
	wchar_t coinui[255];
	wchar_t mutekiui[255];

	swprintf_s(coinui, 255, L"%d枚", m_player->getCoin);
	swprintf_s(mutekiui, 255, L"%d個", m_player->mutekiItem);

	//コインUIのフォント設定
	getCoinUI.SetText(coinui);
	getCoinUI.SetPosition(Vector3(-800.0f, 500.0f, 0.0f));
	getCoinUI.SetScale(2.5f);

	//無敵UIのフォント設定
	mutekiCount.SetText(mutekiui);
	mutekiCount.SetPosition(Vector3(640.0f, -350.0f, 0.0f));
	mutekiCount.SetScale(3.0f);
}

void GameUI::Render(RenderContext& rc)
{
	getCoinUI.Draw(rc);
	coinSprite.Draw(rc);
	sterSprite.Draw(rc);
	mutekiCount.Draw(rc);

	if (m_player->hitMeteo == 3)
	{
		life01Sprite.Draw(rc);
	}
	if (m_player->hitMeteo > 1) {
		life02Sprite.Draw(rc);
	}
	if (m_player->hitMeteo >= 1)
	{
		life03Sprite.Draw(rc);
	}
}
