#include "stdafx.h"
#include "Player.h"
#include "Game.h"
#include "sound/SoundSource.h"
#include "sound/SoundEngine.h"

Player::Player()
{
	animationClips[enAnimationClip_Idle].Load("Assets/animData/playerIdle.tka");
	animationClips[enAnimationClip_Idle].SetLoopFlag(true);
	animationClips[enAnimationClip_Run].Load("Assets/animData/playerRun.tka");
	animationClips[enAnimationClip_Run].SetLoopFlag(true);
	animationClips[enAnimationClip_Hit].Load("Assets/animData/playerHit.tka");
	animationClips[enAnimationClip_Hit].SetLoopFlag(false);
	animationClips[enAnimationClip_Death].Load("Assets/animData/playerDeath.tka");
	animationClips[enAnimationClip_Death].SetLoopFlag(false);
	animationClips[enAnimationClip_Dance].Load("Assets/animData/playerDance.tka");
	animationClips[enAnimationClip_Dance].SetLoopFlag(true);

	m_modelRender.Init("Assets/modelData/mainchar/player.tkm",animationClips, enAnimationClip_Num,enModelUpAxisZ);
	m_charCon.Init(15.0f, 50.0f, m_position);

	m_game = FindGO<Game>("game");

	g_soundEngine->ResistWaveFileBank(10, "Assets/sound/powerup01.wav");

	m_pointLight.Init();
	m_pointLight.SetPosition(m_position);
	m_pointLight.SetColor(Vector3(15.0f, 0.0f, 0.0f));
	m_pointLight.SetAffectPowParam(0.7f);
	m_pointLight.SetRange(100.0f);

}

Player::~Player()
{
}

void Player::Update()
{
	Move();
	Turn();
	PlayAnimation();
	Muteki();

	ManageState();


	//�|�C���g���C�g���L�����N�^�[�ɒǏ]������
	Vector3 pointLightPosition = m_position;
	pointLightPosition.y += 50.0f;
	m_pointLight.SetPosition(pointLightPosition);

	if (g_pad[0]->IsPress(enButtonX))
	{
		m_range += 1.0f;
		
		if (m_range >= 500.0f)
		{
			m_range = 499.0f;
		}
	}
	if (g_pad[0]->IsPress(enButtonY))
	{
		m_range -= 1.0f;
		if (m_range <= 0.0f)
		{
			m_range = 0.1f;
		}
	}
	m_pointLight.SetRange(m_range);

	m_pointLight.Update();
	m_modelRender.Update();
}

void Player::Move()
{
	//�̗͂�0�ɂȂ邩5���o�߂��邩�q�b�g�A�j���[�V�������͈ړ��ł��Ȃ�����
	if (hitMeteo <= 0 || m_game->generate_flag == 5 || meteohit_flag == 1)
	{
		return;
	}

	//���X�e�B�b�N�̓��͗�
	float lStick_x = g_pad[0]->GetLStickXF();
	float lStick_y = g_pad[0]->GetLStickYF();
	//�J�����̑O���ƉE�������擾
	Vector3 cameraForward = g_camera3D->GetForward();
	Vector3 cameraRight = g_camera3D->GetRight();
	//XZ���ʂł̑O�������A�E�����ɕϊ�����
	cameraForward.y = 0.0f;
	cameraForward.Normalize();
	cameraRight.y = 0.0f;
	cameraRight.Normalize();
	//XZ�����̈ړ����x���N���A
	m_moveSpeed.x = 0.0f;
	m_moveSpeed.z = 0.0f;
	m_moveSpeed += cameraForward * lStick_y * 200.0f;
	m_moveSpeed += cameraRight * lStick_x * 200.0f;
	
	m_position = m_charCon.Execute(m_moveSpeed, g_gameTime->GetFrameDeltaTime());
	

	if (m_charCon.IsOnGround())
	{
		m_moveSpeed.y = 0.0f;
	}


	//�v���C���[�̈ړ��\�͈͂̐ݒ�
	//x���ɂ���
	if (m_position.x >= MOVERANGERIGHT || m_position.x <= MOVERANGELEFT)
	{
		if (m_position.x >= MOVERANGERIGHT) {
			m_position.x = MOVERANGERIGHT;
			m_charCon.SetPosition(m_position);
		}
		else {
			m_position.x = MOVERANGELEFT;
			m_charCon.SetPosition(m_position);
		}
	}
	//z���ɂ���
	if (m_position.z >= MOVERANGEFRONT || m_position.z <= MOVERANGEBACK)
	{
		if (m_position.z >= MOVERANGEFRONT) {
			m_position.z = MOVERANGEFRONT;
			m_charCon.SetPosition(m_position);
		}
		else {
			m_position.z = MOVERANGEBACK;
			m_charCon.SetPosition(m_position);
		}
	}

	m_modelRender.SetPosition(m_position);
}

void Player::Turn()
{
	//�̗͂�0�ɂȂ邩5���o���q�b�g�A�j���[�V�������͉�]���Ȃ�����
	if (hitMeteo <= 0 || m_game->generate_flag == 5 || meteohit_flag == 1)
	{
		return;
	}

	if (fabsf(m_moveSpeed.x) < 0.001f && fabsf(m_moveSpeed.z) < 0.001f)
	{
		//�L�������ړ����ĂȂ������]�̕K�v�Ȃ�
		return;
	}

	float angle = atan2(-m_moveSpeed.x, m_moveSpeed.z);

	m_rotation.SetRotationY(-angle);
	//��]�̐ݒ�
	m_modelRender.SetRotation(m_rotation);
}

void Player::Muteki()
{
	if (hitMeteo <= 0 || m_game->generate_flag == 5)
	{
		return;
	}

	//�R�C���l������20�����̏ꍇ�͏������Ȃ�
	if (getCoin < NEEDCOIN)
	{
		return;
	}

	//���G�A�C�e���̌������߂�i�R�C��20����1�j
	if (getCoin % NEEDCOIN == 0 && itemzouka_flag == 0)
	{
		itemzouka_flag = 1;
		mutekiItem++;
	}

	if (g_pad[0]->IsTrigger(enButtonB) && mutekiItem >= 1 && muteki_flag == 0)
	{
		//se�̍Đ�
		SoundSource* se = NewGO<SoundSource>(0);
		se->Init(10);
		se->SetVolume(2.0f);
		se->Play(false);

		mutekiItem--;
		muteki_flag = 1;
	}
}

void Player::ManageState()
{
	//�̗͂�0�ɂȂ�ƃA�j���X�e�[�g��3�i���S�j�ɂ���
	if (hitMeteo <= 0)
	{
		playerState = 3;
		return;
	}
	
	//���Ԃ̍Ō�܂Ő����c��ƃ_���X������
	if (m_game->generate_flag == 5)
	{
		playerState = 4;
		return;
	}

	if (meteohit_flag == 1)
	{
		playerState = 2;
		anim_timer += g_gameTime->GetFrameDeltaTime();
		if (anim_timer >= 0.4f)
		{
			meteohit_flag = 0;
			anim_timer = 0.0f;
		}
		return;
	}
	if (fabsf(m_moveSpeed.x) >= 0.001f || fabsf(m_moveSpeed.z) >= 0.001f)
	{
		playerState = 1;
	}
	else
	{
		playerState = 0;
	}
}

void Player::PlayAnimation()
{
	switch (playerState)
	{
	case 0:
		m_modelRender.PlayAnimation(enAnimationClip_Idle);
		break;
	case 1:
		m_modelRender.PlayAnimation(enAnimationClip_Run);
		break;
	case 2:
		m_modelRender.PlayAnimation(enAnimationClip_Hit);
		break;
	case 3:
		m_modelRender.PlayAnimation(enAnimationClip_Death);
		break;
	case 4:
		m_modelRender.PlayAnimation(enAnimationClip_Dance);
	}
}

void Player::Render(RenderContext& rc)
{
	m_modelRender.Draw(rc);
}
