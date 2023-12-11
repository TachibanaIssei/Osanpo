#include "stdafx.h"
#include "Game.h"
#include "nature/SkyCube.h"
#include "Player.h"
#include "BackGround.h"
#include "GameCamera.h"
#include "Meteo.h"
#include "Coin.h"
#include "GameResult.h"
#include "GameUI.h"
#include "speedUP.h"
#include "sound/SoundSource.h"
#include "sound/SoundEngine.h"
#include "graphics/effect/EffectEmitter.h"
#include "graphics/RenderingEngine.h"

Game::~Game()
{
	DeleteGO(player);
	DeleteGO(backGround);
	DeleteGO(gameUI);
	DeleteGO(gameBGM);
	DeleteGO(gameCamera);
	DeleteGO(m_skyCube);
}

void Game::InitSky()
{
	DeleteGO(m_skyCube);

	m_skyCube = NewGO<SkyCube>(0, "skycube");
	m_skyCube->SetType((EnSkyCubeType)m_skyCubeType);

	dir = { 1, -1, 0 };
	dir.Normalize();
	g_renderingEngine->SetDirectionLight(
		0,					//ライトの番号
		dir,				//方向
		{ 1,1,1 }		//カラー
	);
	g_renderingEngine->SetAmbient({ 1,1,1 });

	m_skyCube->SetLuminance(2.0f);
	m_skyCube->SetType((EnSkyCubeType)m_skyCubeType);
	m_skyCube->SetScale(200.0f);

	// 環境光の計算のためのIBLテクスチャをセットする。
	g_renderingEngine->SetAmbientByIBLTexture(m_skyCube->GetTextureFilePath(), 1.0f);
	// 環境日光の影響が分かりやすいように、ディレクションライトはオフに。
	//g_renderingEngine->SetDirectionLight(0, g_vec3Zero, g_vec3Zero);
}

bool Game::Start()
{
	InitSky();	

	player = NewGO<Player>(0, "player");
	backGround = NewGO<BackGround>(0, "backGround");
	gameCamera = NewGO<GameCamera>(0, "gameCamera");
	gameUI = NewGO<GameUI>(0, "gameUI");

	goResultSprite.Init("Assets/sprite/PLESS_A_BUTTON_yellow.dds", 1920.0f, 1080.0f);
	goResultSprite.SetPosition(Vector3(0.0f, -300.0f, 0.0f));

	mutekiSprite.Init("Assets/sprite/muteki_font.dds", 1920.0f, 1080.0f);
	mutekiSprite.SetPosition(Vector3(0.0f, -280.0f, 0.0f));

	congraSprite.Init("Assets/sprite/congra_font.dds", 1920.0f, 1080.0f);
	congraSprite.SetPosition(Vector3(0.0f, 250.0f, 0.0f));

	thankSprite.Init("Assets/sprite/thankPlaying_font.dds", 1920.0f, 1080.0f);
	thankSprite.SetPosition(Vector3(0.0f, 250.0f, 0.0f));

	//ゲーム中のBGMを読み込む。
	g_soundEngine->ResistWaveFileBank(3, "Assets/sound/gameBGM.wav");
	//ゲーム中のBGMを再生する。
	gameBGM = NewGO<SoundSource>(0);
	gameBGM->Init(3);
	gameBGM->SetVolume(0.5f);
	gameBGM->Play(true);

	g_soundEngine->ResistWaveFileBank(8, "Assets/sound/buttonSE.wav");	//ボタンse
	g_soundEngine->ResistWaveFileBank(11, "Assets/sound/shot.wav");		//オブジェクト生成のse
	g_soundEngine->ResistWaveFileBank(12, "Assets/sound/kansei.wav");	//3分生き残ったときの音
	g_soundEngine->ResistWaveFileBank(13, "Assets/sound/hakushu.wav");	//生き残れなかった時の音

	//エフェクトを読み込む
	EffectEngine::GetInstance()->ResistEffect(0, u"Assets/effect/root.efk");
	EffectEngine::GetInstance()->ResistEffect(1, u"Assets/effect/root02.efk");
	EffectEngine::GetInstance()->ResistEffect(2, u"Assets/effect/root03.efk");


	//スポットライト・ボリュームライトの設定
	m_spotLight.Init();
	m_spotLight.SetRange(600.0f);

	//1本目は、大外まで広がる光
	m_spotLight.SetColor(Vector3{ 50.0f,0.0f,0.0f });
	m_spotLight.SetAngle(Math::DegToRad(15.0f));
	m_spotLight.SetRangeAffectPowParam(2.0f);
	m_spotLight.SetAngleAffectPowParam(2.0f);

	//2本目は根元の光
	m_spotLight.SetColor2(Vector3{ 0.0f,0.0f,100.0f });
	m_spotLight.SetAngle2(Math::DegToRad(15.0f));
	m_spotLight.SetRangeAffectPowParam2(40.0f);
	m_spotLight.SetAngleAffectPowParam2(0.5f);

	//3本目は中心をとおっている光
	m_spotLight.SetColor3(Vector3{0.0f,50.0f,0.0f});
	m_spotLight.SetAngle3(Math::DegToRad(15.0));
	m_spotLight.SetRangeAffectPowParam3(3.0f);
	m_spotLight.SetAngleAffectPowParam3(2.0f);

	Vector3 direction = Vector3::AxisY;
	m_spotLight.SetDirection(direction);
	m_spotLight.SetPosition(Vector3{ 0.0f,10.0f,0.0f });
	m_volumeSpotLight.Init(m_spotLight);

	return true;
}

void Game::Update()
{
	//ディレクションライトの回転
	if (g_pad[0]->IsPress(enButtonRight))
	{
		if (dir.x > 1.1f)
		{
			dir.x = 1.0f;
		}
		else
		{
			dir.x += 0.1f;
		}
	}

	if (g_pad[0]->IsPress(enButtonLeft))
	{
		if (dir.x < -1.1f)
		{
			dir.x = -1.0f;
		}
		else
		{
			dir.x -= 0.1f;
		}
	}
	dir.Normalize();
	g_renderingEngine->SetDirectionLight(
		0,					//ライトの番号
		dir,				//方向
		{ 1,1,1 }		//カラー
	);

	//ボリュームライトの回転
	m_timer += g_gameTime->GetFrameDeltaTime();
	float angle = cosf(m_timer) * 1.7f;
	m_rotation.AddRotationDegZ(-angle);
	Vector3 direction = Vector3::AxisY;
	m_rotation.Apply(direction);
	m_spotLight.SetDirection(direction);

	m_spotLight.Update();
	m_volumeSpotLight.Update();

	//死ぬか５分経過した時の処理
	if (player->hitMeteo <= 0 || generate_flag == 5)
	{
		goResultFlag = 1;
		gameBGM->Stop();

		if (player->hitMeteo <= 0 && endse_flag == 0)
		{
			endse_flag = 1;

			SoundSource* deadse = NewGO<SoundSource>(0);
			deadse->Init(13);
			deadse->SetVolume(0.8f);
			deadse->Play(false);
		}

		if (generate_flag == 5 && endse_flag == 0)
		{
			endse_flag = 1;

			SoundSource* clearse = NewGO<SoundSource>(0);
			clearse->Init(12);
			clearse->SetVolume(0.8f);
			clearse->Play(false);
		}

		//Aボタンを押してresult画面に行く
		if (g_pad[0]->IsTrigger(enButtonA))
		{
			//seの再生
			SoundSource* buttonse = NewGO<SoundSource>(0);
			buttonse->Init(8);
			buttonse->SetVolume(2.0f);
			buttonse->Play(false);

			gameResult = NewGO<GameResult>(0, "gameresult");

			gameResult->timer_m = timer_m; //リザルトのタイマー表示のためにタイムを代入
			gameResult->timer_s = timer_s;
			gameResult->coin_count = player->getCoin; //コインの獲得数をリザルトに持っていく
			DeleteGO(this);
		}
		
		return;
	}
	GameTimer();
	//Generate();	
	
	goResultSprite.Update();
	mutekiSprite.Update();
	congraSprite.Update();
	thankSprite.Update();

	//g_renderingEngine->DisableRaytracing();
}

void Game::Generate() {

	//0:00～
	if (generate_flag == 0) {
		if (object_timer < 1.5f) {
			return;
		}

		object_timer = 0.0f;
		int Game_Object[11] = { 0 };

		//コインの生成処理
		int CoinNo = 0;
		while (CoinNo < 1)
		{
			int pos = rand() % 12;

			if (Game_Object[pos] == 0) {
				Game_Object[pos] = 1;
				CoinNo++;
			}
		}

		//障害物の生成処理
		int MeteoNo = 0;
		while (MeteoNo < 5)
		{
			int pos = rand() % 12;

			if (Game_Object[pos] == 0) {
				Game_Object[pos] = 2;
				MeteoNo++;
			}
		}

		Xpos = -250.0f;
		effectse_flag = 0;
		for (int i = 0; i < 11; i++) {
			if (Game_Object[i] == 1) {
				Coin* coin = NewGO<Coin>(0, "coin");
				coin->m_position.x = Xpos;
			}

			if (Game_Object[i] == 2) {
				Meteo* meteo = NewGO<Meteo>(0, "meteo");
				meteo->m_position.x = Xpos;
			}
			EffectPlay();
			Xpos += 50.0f;
		}
	}

	//0:36～
	if (generate_flag == 1) {
		if (object_timer < 1.3f) {
			return;
		}

		object_timer = 0.0f;
		int Game_Object[11] = { 0 };

		//コインの生成処理
		int CoinNo = 0;
		while (CoinNo < 1)
		{
			int pos = rand() % 12;

			if (Game_Object[pos] == 0) {
				Game_Object[pos] = 1;
				CoinNo++;
			}
		}

		//障害物の生成処理
		int MeteoNo = 0;
		while (MeteoNo < 5)
		{
			int pos = rand() % 12;

			if (Game_Object[pos] == 0) {
				Game_Object[pos] = 2;
				MeteoNo++;
			}
		}

		Xpos = -250.0f;
		effectse_flag = 0;
		for (int i = 0; i < 11; i++) {
			if (Game_Object[i] == 1) {
				Coin* coin = NewGO<Coin>(0, "coin");
				coin->m_position.x = Xpos;
			}
			if (Game_Object[i] == 2) {
				Meteo* meteo = NewGO<Meteo>(0, "meteo");
				meteo->m_position.x = Xpos;
			}
			EffectPlay();
			Xpos += 50.0f;
		}
	}

	//1:12～
	if (generate_flag == 2) {
		if (object_timer < 1.0f) {
			return;
		}

		object_timer = 0.0f;
		int Game_Object[11] = { 0 };

		//コインの生成処理
		int CoinNo = 0;
		while (CoinNo < 2)
		{
			int pos = rand() % 12;

			if (Game_Object[pos] == 0) {
				Game_Object[pos] = 1;
				CoinNo++;
			}
		}

		//障害物の生成処理
		int MeteoNo = 0;
		while (MeteoNo < 6)
		{
			int pos = rand() % 12;

			if (Game_Object[pos] == 0) {
				Game_Object[pos] = 2;
				MeteoNo++;
			}
		}

		Xpos = -250.0f;
		effectse_flag = 0;
		for (int i = 0; i < 11; i++) {
			if (Game_Object[i] == 1) {
				Coin* coin = NewGO<Coin>(0, "coin");
				coin->m_position.x = Xpos;
			}
			if (Game_Object[i] == 2) {
				Meteo* meteo = NewGO<Meteo>(0, "meteo");
				meteo->m_position.x = Xpos;
			}
			EffectPlay03();
			Xpos += 50.0f;
		}
	}

	//1:48～
	if (generate_flag == 3) {
		if (object_timer < 0.8f) {
			return;
		}

		object_timer = 0.0f;
		int Game_Object[11] = { 0 };

		//コインの生成処理
		int CoinNo = 0;
		while (CoinNo < 2)
		{
			int pos = rand() % 12;

			if (Game_Object[pos] == 0) {
				Game_Object[pos] = 1;
				CoinNo++;
			}
		}

		//障害物の生成処理
		int MeteoNo = 0;
		while (MeteoNo < 6)
		{
			int pos = rand() % 12;

			if (Game_Object[pos] == 0) {
				Game_Object[pos] = 2;
				MeteoNo++;
			}
		}

		Xpos = -250.0f;
		effectse_flag = 0;
		for (int i = 0; i < 11; i++) {
			if (Game_Object[i] == 1) {
				Coin* coin = NewGO<Coin>(0, "coin");
				coin->m_position.x = Xpos;
			}
			if (Game_Object[i] == 2) {
				Meteo* meteo = NewGO<Meteo>(0, "meteo");
				meteo->m_position.x = Xpos;
			}
			EffectPlay03();
			Xpos += 50.0f;
		}
	}

	//2:24～
	if (generate_flag == 4) {
		if (object_timer < 0.7f) {
			return;
		}

		object_timer = 0.0f;
		int Game_Object[11] = { 0 };

		//コインの生成処理
		int CoinNo = 0;
		while (CoinNo < 3)
		{
			int pos = rand() % 12;

			if (Game_Object[pos] == 0) {
				Game_Object[pos] = 1;
				CoinNo++;
			}
		}

		//障害物の生成処理
		int MeteoNo = 0;
		while (MeteoNo < 7)
		{
			int pos = rand() % 12;

			if (Game_Object[pos] == 0) {
				Game_Object[pos] = 2;
				MeteoNo++;
			}
		}

		Xpos = -250.0f;
		effectse_flag = 0;
		for (int i = 0; i < 11; i++) {
			if (Game_Object[i] == 1) {
				Coin* coin = NewGO<Coin>(0, "coin");
				coin->m_position.x = Xpos;
			}
			if (Game_Object[i] == 2) {
				Meteo* meteo = NewGO<Meteo>(0, "meteo");
				meteo->m_position.x = Xpos;
			}
			EffectPlay03();
			Xpos += 50.0f;
		}
	}
}

//タイマーの処理
void Game::GameTimer()
{
	wchar_t wcsbuf[255];
	swprintf_s(wcsbuf, 255, L"%d:%02d", int(timer_m) / 60, int(timer_s));

	//時間の表示
	m_fontRender.SetText(wcsbuf);
	m_fontRender.SetPosition(Vector3(-100.0f, 500.0f, 0.0f));
	m_fontRender.SetScale(2.0f);

	timer_s += g_gameTime->GetFrameDeltaTime();
	timer_m += g_gameTime->GetFrameDeltaTime();
	object_timer += g_gameTime->GetFrameDeltaTime();
	
	//無敵フラグが１になったときに時間を計測する
	if (player->muteki_flag == 1)
	{
		muteki_timer += g_gameTime->GetFrameDeltaTime();

		if (int(muteki_timer) == 1 && mutekiCountFlag == 0)
		{
			muteki_count--;
			mutekiCountFlag = 1;
		}
		else if (int(muteki_timer) == 2 && mutekiCountFlag == 1)
		{
			muteki_count--;
			mutekiCountFlag = 2;
		}
		else if (int(muteki_timer) == 3 && mutekiCountFlag == 2)
		{
			muteki_count--;
			mutekiCountFlag = 3;
		}

		wchar_t muteki[255];
		swprintf_s(muteki, 255, L"%d", int(muteki_count));

		//無敵時間の表示
		mutekiTimeRender.SetText(muteki);
		mutekiTimeRender.SetPosition(Vector3(-50.0f, -345.0f, 0.0f));
		mutekiTimeRender.SetScale(2.5f);
		mutekiTimeRender.SetColor(g_vec4Black);
	}
	

	switch (int(timer_m))
	{
	case 0: //0:00
		generate_flag = 0;
		break;
	case 36: //0:36
		generate_flag = 1;
		if (speedup_flag == 0)
		{
			NewGO<speedUP>(0, "speedUP");
			speedup_flag = 1;
		}
		break;
	case 72: //1:12
		generate_flag = 2;
		if (speedup_flag == 1)
		{
			NewGO<speedUP>(0, "speedUP");
			speedup_flag = 2;
		}
		break;
	case 108: //1:48
		generate_flag = 3;
		if (speedup_flag == 2)
		{
			NewGO<speedUP>(0, "speedUP");
			speedup_flag = 3;
		}
		break;
	case 144: //2:24
		generate_flag = 4;
		if (speedup_flag == 3)
		{
			NewGO<speedUP>(0, "speedUP");
			speedup_flag = 4;
		}
		break;
	case 181: //3:00
		generate_flag = 5;
		if (timehiku_flag == 0)
		{
			timer_m -= 1.0f;
			timehiku_flag = 1;
		}
		break;
	}

	if (timer_s >= 60.0f) {
		timer_s = 0.0f;
	}

	//無敵タイマーが３秒以上になったらリセットする
	if (muteki_timer >= 3.0f)
	{
		muteki_timer = 0.0f;
		player->muteki_flag = 0;
		mutekiCountFlag = 0;
		muteki_count = 3;
	}
}

void Game::EffectPlay()
{
	EffectEmitter* effectEmitter = NewGO<EffectEmitter>(0);
	effectEmitter->Init(0);
	effectEmitter->SetPosition(Vector3(Xpos,3.0f,500.0f));
	effectEmitter->SetScale({ 15.0f,15.0f,15.0f });
	effectEmitter->Play();

	if (effectse_flag == 0)
	{
		SoundSource* effectse = NewGO<SoundSource>(0);
		effectse->Init(11);
		effectse->SetVolume(0.3f);
		effectse->Play(false);

		effectse_flag = 1;
	}
}

void Game::EffectPlay02()
{
	EffectEmitter* effectEmitter = NewGO<EffectEmitter>(0);
	effectEmitter->Init(1);
	effectEmitter->SetPosition(Vector3(Xpos, 3.0f, 500.0f));
	effectEmitter->SetScale({ 15.0f,15.0f,15.0f });
	effectEmitter->Play();

	if (effectse_flag == 0)
	{
		SoundSource* effectse = NewGO<SoundSource>(0);
		effectse->Init(11);
		effectse->SetVolume(0.3f);
		effectse->Play(false);

		effectse_flag = 1;
	}
}

void Game::EffectPlay03()
{
	EffectEmitter* effectEmitter = NewGO<EffectEmitter>(0);
	effectEmitter->Init(2);
	effectEmitter->SetPosition(Vector3(Xpos, 3.0f, 400.0f));
	effectEmitter->SetScale({ 15.0f,15.0f,15.0f });
	effectEmitter->Play();

	if (effectse_flag == 0)
	{
		SoundSource* effectse = NewGO<SoundSource>(0);
		effectse->Init(11);
		effectse->SetVolume(0.3f);
		effectse->Play(false);

		effectse_flag = 1;
	}
}

void Game::Render(RenderContext& rc)
{
	m_fontRender.Draw(rc);	
	
	if (goResultFlag == 1)
	{
		goResultSprite.Draw(rc);

		if (player->hitMeteo <= 0)
		{
			thankSprite.Draw(rc);
		}
		else {
			congraSprite.Draw(rc);
		}
	}

	if (player->muteki_flag == 1 && generate_flag != 5)
	{
		mutekiSprite.Draw(rc);
		mutekiTimeRender.Draw(rc);
	}
}