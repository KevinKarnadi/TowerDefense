#ifndef ICEENEMY_HPP
#define ICEENEMY_HPP
#include "Enemy.hpp"

class IceEnemy : public Enemy
{
public:
	IceEnemy(int x, int y);
	void CreateBullet() override;
};
#endif // ICEENEMY_HPP

