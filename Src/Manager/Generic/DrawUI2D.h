#pragma once
#include "../Object/Common/Transform.h"
#include "DrawUIBase.h"
class DrawUI2D :public DrawUIBase
{
	DrawUI2D(void);
	~DrawUI2D(void)override;
	void Load(void)override;
	void Init(void)override;
	void Update(void)override;
	void Draw(void)override;
private:
};

