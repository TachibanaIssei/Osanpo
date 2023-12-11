#pragma once

class Player;
class Game;

class Meteo : public IGameObject
{
public:
	Meteo();
	~Meteo();
	void Update();
	void Move();
	void Rotation();
	void Render(RenderContext& rc);

	ModelRender m_modelRender;
	Vector3 m_position;
	Player* m_player;
	Game* m_game;
	Quaternion m_rotation;

	const float PLAYERHIT = 38.0f;			//プレイヤーが石に当たる距離の数値
	const float PLAYERNOTHIT = -650.0f;		//石がプレイヤーに当たらず消える距離の数値
};

