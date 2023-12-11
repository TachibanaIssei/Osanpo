#pragma once
class Game;
class Player : public IGameObject
{
public:
	Player();
	~Player();
	void Update();
	void Render(RenderContext& rc);
	void Move();
	void Turn();
	void Muteki();
	void ManageState();
	void PlayAnimation();

	ModelRender m_modelRender;
	Vector3 m_position;
	enum EnAnimationClip {
		enAnimationClip_Idle,
		enAnimationClip_Run,
		enAnimationClip_Hit,
		enAnimationClip_Death,
		enAnimationClip_Dance,
		enAnimationClip_Num,
	};
	AnimationClip animationClips[enAnimationClip_Num];
	CharacterController m_charCon;
	Vector3 m_moveSpeed;
	Quaternion m_rotation;

	Game* m_game=nullptr;

	PointLight m_pointLight;
	float m_range = 0.0f;

	int playerState = 0;
	int getCoin = 0;
	int hitMeteo = 3;
	int mutekiItem = 0;
	int itemzouka_flag = 0;
	int muteki_flag = 0;
	int meteohit_flag = 0;
	float anim_timer = 0.0f;

	const int NEEDCOIN = 40;				//アイテム取得に必要なコインの枚数の数値
	const float MOVERANGERIGHT = 270.0f;	//X軸移動可能範囲右側の数値
	const float MOVERANGELEFT = -270.0f;	//X軸移動可能範囲左側の数値
	const float MOVERANGEFRONT = -250.0f;	//Z軸移動可能範囲前側の数値
	const float MOVERANGEBACK = -380.0f;	//Z軸移動可能範囲後側の数値
};

