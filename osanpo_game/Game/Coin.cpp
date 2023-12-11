#include "stdafx.h"
#include "Coin.h"

#include "Player.h"
#include "Game.h"
#include "sound/SoundSource.h"
#include "sound/SoundEngine.h"
#include "graphics/effect/EffectEmitter.h"

Coin::Coin()
{
	m_modelRender.SetRaytracingWorld(false);

	m_modelRender.Init("Assets/modelData/coin.tkm");

	m_position.y = 25.0f;
	m_position.z = 700.0f;

	m_modelRender.SetPosition(m_position);
	m_modelRender.Update();

	m_player = FindGO<Player>("player");
	m_game = FindGO<Game>("game");

	g_soundEngine->ResistWaveFileBank(1, "Assets/sound/coinget.wav");

	//エフェクトを読み込む
	EffectEngine::GetInstance()->ResistEffect(3, u"Assets/effect/coinget.efk");
}

Coin::~Coin()
{

}

void Coin::Update()
{
	if (m_player->hitMeteo <= 0 || m_game->generate_flag == 5)
	{
		DeleteGO(this);
		return;
	}

	Move();
	Rotation();

	//プレイヤーが当たらなかった時の処理
	if (m_position.z <= -650.0f)
	{
		DeleteGO(this);
	}

	//プレイヤーが当たった場合の処理
	Vector3 diff = m_player->m_position - m_position;
	if (diff.Length() <= 45.0f)
	{
		//エフェクトの再生
		EffectEmitter* effectEmitter = NewGO<EffectEmitter>(0);
		effectEmitter->Init(3);
		effectEmitter->SetPosition(m_position);
		effectEmitter->SetScale({ 10.0f,10.0f,10.0f });
		effectEmitter->Play();

		//seの再生
		SoundSource* se = NewGO<SoundSource>(0);
		se->Init(1);
		se->SetVolume(0.5);
		se->Play(false);

		m_player->itemzouka_flag = 0;
		m_player->getCoin++;

		DeleteGO(this);
	}

	m_modelRender.Update();
}

void Coin::Move()
{
	int generate_flag = m_game->generate_flag;
	switch (m_game->generate_flag)
	{
	case 0:
		m_position.z -= 5.0f;
		break;
	case 1:
		m_position.z -= 5.5f;
		break;
	case 2:
		m_position.z -= 6.0f;
		break;
	case 3:
		m_position.z -= 6.5f;
		break;
	case 4:
		m_position.z -= 7.0f;
		break;
	}

	m_modelRender.SetPosition(m_position);
}

void Coin::Rotation()
{
	m_rotation.AddRotationDegY(2.0f);
	m_modelRender.SetRotation(m_rotation);
}

void Coin::Render(RenderContext& rc)
{
	m_modelRender.Draw(rc);
}
