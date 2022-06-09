#ifndef WBCELLTURRET_HPP
#define WBCELLTURRET_HPP
#include "Turret.hpp"

class WBCellTurret: public Turret
{
public:
	static const int Price;
	static const int hp;
    WBCellTurret(float x, float y);
	void CreateBullet() override;
};
#endif // WBCELLTURRET_HPP
