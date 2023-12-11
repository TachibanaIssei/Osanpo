#include "stdafx.h"
#include "speedUP.h"
#include "sound/SoundSource.h"
#include "sound/SoundEngine.h"

speedUP::speedUP()
{
	//se‚ÌÄ¶
	g_soundEngine->ResistWaveFileBank(2, "Assets/sound/speedup.wav");
	SoundSource* se = NewGO<SoundSource>(0);
	se->Init(2);
	se->Play(false);

	spritRender.Init("Assets/sprite/speedUP.dds", 666.0f, 374.0f);
	position = { 800.0f,290.0f,0.0f };
	spritRender.SetPosition(position);
	spritRender.SetScale(Vector3{2.0f,2.0f,1.0f});
}

speedUP::~speedUP()
{

}

void speedUP::Update()
{
	if (position.x <= -1100.0f)
	{
		DeleteGO(this);
	}

	position.x -= 10.0f;
	spritRender.SetPosition(position);
	spritRender.Update();
}

void speedUP::Render(RenderContext& rc)
{
	spritRender.Draw(rc);
}
