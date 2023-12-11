#pragma once

class Player;
class Game;

class Coin : public IGameObject
{
public:
	Coin();
	~Coin();
	void Update();
	void Move();
	void Rotation();
	void Render(RenderContext& rc);

	ModelRender m_modelRender;
	Vector3 m_position;
	Quaternion m_rotation;
	Player* m_player = nullptr;
	Game* m_game = nullptr;

	int effect_flag = 0;
};

