#pragma once
class DrawUIBase
{
public:
	DrawUIBase(void);
	virtual ~DrawUIBase(void);
	virtual void Load(void) = 0;
	virtual void Init(void)=0;
	virtual void Update(void) = 0;
	virtual void Draw(void) = 0;
};

