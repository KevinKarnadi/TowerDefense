#ifndef IDLETURRET_HPP
#define IDLETURRET_HPP
#include "Turret.hpp"

class IdleTurret : public Turret
{
public:
	static const int Price;
	static const float hp;
	IdleTurret(float x, float y);
	void CreateBullet() override;
};
#endif // IDLETURRET_HPP
