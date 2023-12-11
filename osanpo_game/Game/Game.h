#pragma once
#include "Level3DRender/LevelRender.h"
#include "sound/SoundSource.h"

class Player;
class BackGround;
class GameCamera;
class Coin;
class GameUI;
class GameResult;

class Game : public IGameObject
{
private:
	void InitSky();
	SkyCube* m_skyCube = nullptr;
	int m_skyCubeType = enSkyCubeType_Day;
public:
	Game() {}
	~Game();
	bool Start();
	void Update();
	void Generate();
	void GameTimer();
	void EffectPlay();		//0:00～2:59までのエフェクト
	void EffectPlay02();	//それ以降のエフェクト
	void EffectPlay03();
	void Render(RenderContext& rc);

	Player* player = nullptr;
	BackGround* backGround = nullptr;
	GameCamera* gameCamera = nullptr;
	GameUI* gameUI = nullptr;
	GameResult* gameResult = nullptr;
	SoundSource* gameBGM = nullptr;

	FontRender m_fontRender;
	FontRender mutekiTimeRender;
	SpriteRender goResultSprite;
	SpriteRender mutekiSprite;
	SpriteRender congraSprite;
	SpriteRender thankSprite;

	float object_timer = 0.0f;	//オブジェクトを生成するタイマー
	float muteki_timer = 0.0f;	//無敵時間を計測するタイマー
	float timer_s = 0.0f;		//秒を表示するタイマー
	float timer_m = 0.0f;		//分を表示するタイマー
	float Xpos = -250.0f;		//オブジェクト生成位置
	int generate_flag = 0;
	int goResultFlag = 0;
	int muteki_count = 3;
	int mutekiCountFlag = 0;
	int speedup_flag = 0;
	int effectse_flag = 0;		//オブジェクト生成エフェクトの効果音のフラグ
	int endse_flag = 0;
	int timehiku_flag = 0;

	int kaiten_flag = 0;
	Vector3 dir;

	SpotLight			m_spotLight;
	VolumeSpotLight		m_volumeSpotLight;
	Quaternion			m_rotation;
	Vector3				m_direction;
	float						m_timer = 1.7f;
	int kaiten_flag1 = 0;
};