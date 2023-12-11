#pragma once
class speedUP : public IGameObject
{
public:
	speedUP();
	~speedUP();
	void Update();
	void Render(RenderContext& rc);

	SpriteRender spritRender;
	Vector3 position;
};

