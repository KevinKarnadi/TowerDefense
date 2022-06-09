#include <string>

#include "SofaEnemy.hpp"

#include "EnemyBullet.hpp"
#include "PlayScene.hpp"
#include "AudioHelper.hpp"

SofaEnemy::SofaEnemy(int x, int y) : Enemy("play/enemy-2.png", x, y, 16, 100, 10, 10, 2)
{
	// Use bounding circle to detect collision is for simplicity, pixel-perfect collision can be implemented quite easily,
	// and efficiently if we use AABB collision detection first, and then pixel-perfect collision.
}

void SofaEnemy::CreateBullet()
{
	Engine::Point diff = Engine::Point(-1, 0);
	float rotation = ALLEGRO_PI / 2;
	getPlayScene()->BulletGroup->AddNewObject(new EnemyBullet("play/bullet-9.png", 500, 1, Position, diff, -rotation, this));
	AudioHelper::PlayAudio("missile.wav");
}