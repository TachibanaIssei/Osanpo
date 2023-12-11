#pragma once

class Player;

class GameUI : public IGameObject
{
public:
	GameUI();
	~GameUI();
	void Update();
	void UI();
	void Render(RenderContext& rc);

	Player* m_player;

	FontRender getCoinUI;
	FontRender mutekiCount;
	SpriteRender coinSprite;
	SpriteRender sterSprite;
	SpriteRender life01Sprite;
	SpriteRender life02Sprite;
	SpriteRender life03Sprite;
};

