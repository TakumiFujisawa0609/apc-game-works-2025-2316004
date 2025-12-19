#pragma once
class EnemyHpUI
{
public:
	EnemyHpUI(float& _hpPer);
	~EnemyHpUI(void);
	void Load(void);
	void Init(void);
	void Update(void);
	void Draw(void);
private:
	// ƒvƒŒƒCƒ„[‚Ì‘Ì—Í‚ÌŠ„‡
	float& enemyHpPer_;
};

