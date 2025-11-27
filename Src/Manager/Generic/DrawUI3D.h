#pragma once
#include "../Object/Common/Transform.h"
#include "DrawUIBase.h"
class Camera;

class DrawUI3D :public DrawUIBase
{
public:
	DrawUI3D(std::weak_ptr<Camera> _camera);
	~DrawUI3D(void)override;
	void Load(void)override;
	void Init(void)override;
	void Update(void)override;
	void Draw(void)override;
private:
	//テスト用
	Transform testTrans_;

	std::weak_ptr<Camera> camera_;
};

