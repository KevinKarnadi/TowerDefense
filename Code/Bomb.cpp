#include <allegro5/base.h>
#include <cmath>
#include <string>

#include "AudioHelper.hpp"
#include "Group.hpp"
#include "Bomb.hpp"
#include "PlayScene.hpp"
#include "Point.hpp"
#include "Enemy.hpp"

#include "LOG.hpp"

const int Bomb::Price = 0;
const float Bomb::hp = 0;

Bomb::Bomb(float x, float y) :
	// TODO 2 (2/8): You can imitate the 2 files: 'WBCellTurret.hpp', 'WBCellTurret.cpp' to create a new turret.
	Turret("play/bomb.png", x, y, Price, 0, hp)
{
	// Move center downward, since we the turret head is slightly biased upward.
	Anchor.y += 8.0f / GetBitmapHeight();
}

void Bomb::CreateBullet()
{
}

void Bomb::OnPlacement(int xx, int yy)
{
	OnExplode();
	getPlayScene()->TowerGroup->RemoveObject(objectIterator);
	AudioHelper::PlayAudio("explosion.wav");
	//Engine::LOG(Engine::DEBUGGING) << "		Masuk";
	PlayScene* scene = getPlayScene();
	for (auto& it : scene->TowerGroup->GetObjects())
	{
		Turret* turret = dynamic_cast<Turret*>(it);
		if (turret->Position.x == xx && turret->Position.y == yy)
		{
			turret->Hit(1000);
			/*
			turret->OnExplode();
			// Remove all enemy's reference to target.
			for (auto& temp : turret->lockedEnemies)
				temp->Target = nullptr;
			getPlayScene()->TowerGroup->RemoveObject(objectIterator);
			AudioHelper::PlayAudio("explosion.wav");
			break;
			*/
		}
	}
}