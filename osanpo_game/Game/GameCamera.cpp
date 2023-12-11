#include "stdafx.h"
#include "GameCamera.h"

#include "Player.h"

GameCamera::GameCamera()
{

}

GameCamera::~GameCamera()
{

}

bool GameCamera::Start()
{
	m_toCameraPos.Set(0.0f, 50.0f, -200.0f);
	m_player = FindGO<Player>("player");
	
	m_springCamera.Init(
		*g_camera3D,
		1000.0f,
		true,
		5.0f
	);

	return true;
}

void GameCamera::Update()
{
	Vector3 target = m_player->m_position;
	target.y += 80.0f;
	target += g_camera3D->GetForward() * 20.0f;

	Vector3 toCameraPosOld = m_toCameraPos;
	float x = g_pad[0]->GetRStickXF();
	float y = g_pad[0]->GetRStickYF();

	Quaternion qRot;
	qRot.SetRotationDeg(Vector3::AxisY, 2.0f * x);
	qRot.Apply(m_toCameraPos);

	Vector3 axisX;
	axisX.Cross(Vector3::AxisY, m_toCameraPos);
	axisX.Normalize();
	qRot.SetRotationDeg(axisX, 2.0f * y);
	qRot.Apply(m_toCameraPos);
	
	Vector3 toPosDir = m_toCameraPos;
	toPosDir.Normalize();
	if (toPosDir.y < -0.5f)
	{
		m_toCameraPos = toCameraPosOld;
	}
	else if (toPosDir.y > 0.8f)
	{
		m_toCameraPos = toCameraPosOld;
	}

	Vector3 pos = target + m_toCameraPos;

	//メインカメラに注視点と視点を設定する。
	m_springCamera.SetPosition(pos);
	m_springCamera.SetTarget(target);

	//カメラの更新。
	m_springCamera.Update();
}
