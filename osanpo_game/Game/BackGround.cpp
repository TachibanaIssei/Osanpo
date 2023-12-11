#include "stdafx.h"
#include "BackGround.h"

BackGround::BackGround()
{
	modelRender.Init("Assets/bg/back.tkm");

	//ステージの回転
	//Quaternion rot;
	//rot.SetRotationDegY(180.0f);
	//modelRender.SetRotation(rot);
	
	physicsStaticObject.CreateFromModel(modelRender.GetModel(),
		modelRender.GetModel().GetWorldMatrix());
	//当たり判定の可視化
	//PhysicsWorld::GetInstance()->EnableDrawDebugWireFrame();
	modelRender.Update();
}

BackGround::~BackGround()
{

}

void BackGround::Render(RenderContext& rc)
{
	modelRender.Draw(rc);
}
