#ifndef HEADENEMY_HPP
#define HEADENEMY_HPP
#include "Enemy.hpp"

class HeadEnemy : public Enemy
{
public:
	HeadEnemy(int x, int y);
	void CreateBullet() override;
};
#endif // HEADENEMY_HPP
