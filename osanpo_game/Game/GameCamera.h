#pragma once
#include "camera/SpringCamera.h"
class Player;

class GameCamera : public IGameObject
{
public:
	GameCamera();
	~GameCamera();
	bool Start();
	void Update();

	Player* m_player = nullptr;
	Vector3 m_toCameraPos;
	SpringCamera m_springCamera;
};

