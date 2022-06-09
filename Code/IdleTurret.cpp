#include <allegro5/base.h>
#include <cmath>
#include <string>

#include "AudioHelper.hpp"
#include "Group.hpp"
#include "IdleTurret.hpp"
#include "PlayScene.hpp"
#include "Point.hpp"
#include "Enemy.hpp"

const int IdleTurret::Price = 40;
const float IdleTurret::hp = 20;

IdleTurret::IdleTurret(float x, float y) :
	// TODO 2 (2/8): You can imitate the 2 files: 'WBCellTurret.hpp', 'WBCellTurret.cpp' to create a new turret.
	Turret("play/turret-4.png", x, y, Price, 0, hp)
{
	// Move center downward, since we the turret head is slightly biased upward.
	Anchor.y += 8.0f / GetBitmapHeight();
}

void IdleTurret::CreateBullet()
{
	/*
	Engine::Point diff = Engine::Point(1, 0);
	float rotation = ALLEGRO_PI / 2;
	getPlayScene()->BulletGroup->AddNewObject(new FireBullet(Position, diff, rotation, this));
	AudioHelper::PlayAudio("missile.wav");
	*/
}
