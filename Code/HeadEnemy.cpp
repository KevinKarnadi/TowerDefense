#include <string>

#include "HeadEnemy.hpp"

#include "EnemyBullet.hpp"
#include "PlayScene.hpp"
#include "AudioHelper.hpp"

HeadEnemy::HeadEnemy(int x, int y) : Enemy("play/enemy-4.png", x, y, 10, -50, 5, 5, 2)
{
	// TODO 2 (6/8): You can imitate the 2 files: 'NormalEnemy.hpp', 'NormalEnemy.cpp' to create a new enemy.
	id = 1;
}

void HeadEnemy::CreateBullet()
{
	Engine::Point diff = Engine::Point(1, 0);
	float rotation = ALLEGRO_PI / 2;
	getPlayScene()->BulletGroup->AddNewObject(new EnemyBullet("play/bullet-9.png", 500, 1, Position, diff, rotation, this));
	AudioHelper::PlayAudio("missile.wav");
}