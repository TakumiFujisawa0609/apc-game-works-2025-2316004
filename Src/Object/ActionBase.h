#pragma once
class ActionBase
{
public:
	ActionBase(void);
	virtual ~ActionBase() = default;

	virtual void Init(void) = 0;
	virtual void Update(void) = 0;
	
	void ChangeAction(ActionBase* action);
protected:
};

