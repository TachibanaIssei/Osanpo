#include "stdafx.h"
#include "Meteo.h"

#include "Player.h"
#include "Game.h"
#include "sound/SoundSource.h"
#include "sound/SoundEngine.h"

Meteo::Meteo()
{
	m_modelRender.SetRaytracingWorld(false);

	m_modelRender.Init("Assets/modelData/meteo.tkm");

	m_position.y = 25.0f;
	m_position.z = 700.0f;

	m_modelRender.SetPosition(m_position);
	m_modelRender.Update();

	m_player = FindGO<Player>("player");
	m_game = FindGO<Game>("game");

	g_soundEngine->ResistWaveFileBank(0, "Assets/sound/meteo.wav");
}

Meteo::~Meteo()
{

}

void Meteo::Update()
{
	if (m_player->hitMeteo <= 0 || m_game->generate_flag == 5)
	{
		DeleteGO(this);
		return;
	}

	Move();
	Rotation();

	//�v���C���[��������Ȃ��������̏���
	if (m_position.z <= PLAYERNOTHIT)
	{
		DeleteGO(this);
	}

	//�v���C���[�����������ꍇ�̏���
	Vector3 diff = m_player->m_position - m_position;
	if (diff.Length() <= PLAYERHIT)
	{
		//���G�ł͂Ȃ��Ƃ�
		if (m_player->muteki_flag == 0) {
			//se�̍Đ�
			SoundSource* se = NewGO<SoundSource>(0);
			se->Init(0);
			se->SetVolume(1.5f);
			se->Play(false);

			m_player->hitMeteo--;
			m_player->meteohit_flag = 1;
			DeleteGO(this);
		}
		//���G�̎�
		else
		{
			DeleteGO(this);
		}
	}
	m_modelRender.Update();
}

void Meteo::Move()
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

void Meteo::Rotation()
{
	m_rotation.AddRotationDegX(-2.0f);
	m_modelRender.SetRotation(m_rotation);
}

void Meteo::Render(RenderContext& rc)
{
	m_modelRender.Draw(rc);
}
