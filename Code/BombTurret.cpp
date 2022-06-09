#include <allegro5/base.h>
#include <cmath>
#include <string>

#include <random>
#include <allegro5/allegro_primitives.h>
#include <allegro5/color.h>

#include "AudioHelper.hpp"
#include "Group.hpp"
#include "BombTurret.hpp"
#include "PlayScene.hpp"
#include "Point.hpp"
#include "Enemy.hpp"

#include "DirtyEffect.hpp"
#include "ExplosionEffect.hpp"

const int BombTurret::Price = 100;
const float BombTurret::hp = 1;

BombTurret::BombTurret(float x, float y) :
	// TODO 2 (2/8): You can imitate the 2 files: 'WBCellTurret.hpp', 'WBCellTurret.cpp' to create a new turret.
	Turret("play/turret-5.png", x, y, Price, 0, hp)
{
	// Move center downward, since we the turret head is slightly biased upward.
	Anchor.y += 8.0f / GetBitmapHeight();
}

void BombTurret::CreateBullet()
{
	/*
	Engine::Point diff = Engine::Point(1, 0);
	float rotation = ALLEGRO_PI / 2;
	getPlayScene()->BulletGroup->AddNewObject(new FireBullet(Position, diff, rotation, this));
	AudioHelper::PlayAudio("missile.wav");
	*/
}

void BombTurret::OnExplode()
{
	getPlayScene()->EffectGroup->AddNewObject(new ExplosionEffect(Position.x, Position.y));
	std::random_device dev;
	std::mt19937 rng(dev());
	std::uniform_int_distribution<std::mt19937::result_type> distId(1, 3);
	std::uniform_int_distribution<std::mt19937::result_type> dist(1, 20);
	for (int i = 0; i < 10; i++)
	{
		// Random add 10 dirty effects.
		getPlayScene()->GroundEffectGroup->AddNewObject(new DirtyEffect("play/dirty-" + std::to_string(distId(rng)) + ".png", dist(rng), Position.x, Position.y));
	}

	PlayScene* scene = getPlayScene();
	for (auto& it : scene->EnemyGroup->GetObjects())
	{
		Enemy* enemy = dynamic_cast<Enemy*>(it);
		float a1 = Position.x;
		float b1 = Position.y;
		float a2 = enemy->Position.x;
		float b2 = enemy->Position.y;
		float c = 2.5 * PlayScene::BlockSize;
		if (pow(a2 - a1, 2) + pow(b2 - b1, 2) <= pow(c, 2))
		{
			enemy->Hit(1000);
		}
	}
}