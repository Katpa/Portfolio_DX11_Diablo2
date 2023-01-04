#pragma once

class Diablo : public Monster
{
private:
	const float PI_ANGLE = 0.0625f;

public:
	Diablo();
	~Diablo();

	virtual void Update() override;
	virtual void Render() override;

protected:
	virtual void Control() override;
	void SetClip();

	void MoveTo();
	void Attack();
	void Cast();
	void Cast2();
	void Wait();
	void EndDeath() { isDead = true; }

	void EndTask() { isTaskOver = true; }

	void FireLightening();
	void LighteningSound();
	void FireStomp();

	void PlayDeath() { SFX->Play("diabloDeath", 0.1f); }
};