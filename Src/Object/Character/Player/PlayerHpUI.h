#pragma once
class PlayerHpUI
{
public:
	PlayerHpUI(float& _hpPer);
	~PlayerHpUI(void);
	void Load(void);
	void Init(void);
	void Update(void);
	void Draw(void);
private:
	// ƒvƒŒƒCƒ„[‚Ì‘Ì—Í‚ÌŠ„‡
	float& playerHpPer_;	
};

