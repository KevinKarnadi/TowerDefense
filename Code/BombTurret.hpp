#ifndef BOMBTURRET_HPP
#define BOMBTURRET_HPP
#include "Turret.hpp"

class BombTurret : public Turret
{
public:
	static const int Price;
	static const float hp;
	BombTurret(float x, float y);
	void CreateBullet() override;
	void OnExplode() override;
};
#endif // BOMBTURRET_HPP
