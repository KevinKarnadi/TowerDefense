#include <allegro5/base.h>
#include <cmath>
#include <string>

#include "AudioHelper.hpp"
#include "FireBullet.hpp"
#include "Group.hpp"
#include "FireTurret.hpp"
#include "PlayScene.hpp"
#include "Point.hpp"
#include "Enemy.hpp"

const int FireTurret::Price = 50;
const float FireTurret::hp = 4;

FireTurret::FireTurret(float x, float y) :
	// TODO 2 (2/8): You can imitate the 2 files: 'WBCellTurret.hpp', 'WBCellTurret.cpp' to create a new turret.
	Turret("play/turret-3.png", x, y, Price, 0.8, hp)
{
	// Move center downward, since we the turret head is slightly biased upward.
	Anchor.y += 8.0f / GetBitmapHeight();
}

void FireTurret::CreateBullet()
{
	Engine::Point diff1 = Engine::Point(1, 0);
	Engine::Point diff2 = Engine::Point(-1, 0);
	float rotation = ALLEGRO_PI / 2;
	getPlayScene()->BulletGroup->AddNewObject(new FireBullet(Position, diff1, rotation, this));
	getPlayScene()->BulletGroup->AddNewObject(new FireBullet(Position, diff2, rotation, this));
	AudioHelper::PlayAudio("missile.wav");
}
