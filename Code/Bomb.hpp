#ifndef BOMB_HPP
#define BOMB_HPP
#include "Turret.hpp"

class Bomb : public Turret
{
public:
	static const int Price;
	static const float hp;
	Bomb(float x, float y);
	void CreateBullet() override;
	void OnPlacement(int x, int y);
};
#endif // BOMB_HPP
