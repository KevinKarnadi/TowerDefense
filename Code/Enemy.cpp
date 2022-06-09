#include <allegro5/allegro_primitives.h>
#include <allegro5/color.h>
#include <cmath>
#include <random>
#include <string>
#include <vector>

#include "AudioHelper.hpp"
#include "Bullet.hpp"
#include "DirtyEffect.hpp"
#include "Enemy.hpp"
#include "ExplosionEffect.hpp"
#include "GameEngine.hpp"
#include "Group.hpp"
#include "IScene.hpp"
#include "LOG.hpp"
#include "PlayScene.hpp"
#include "Turret.hpp"

#include "EnemyBullet.hpp"
#include "Collider.hpp"

PlayScene* Enemy::getPlayScene()
{
	return dynamic_cast<PlayScene*>(Engine::GameEngine::GetInstance().GetActiveScene());
}

void Enemy::OnExplode()
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
}

Enemy::Enemy(std::string img, float x, float y, float radius, float speed, float hp, int money, float coolDown) :
	Engine::Sprite(img, x, y), speed(speed), hp(hp), money(money), coolDown(coolDown)
{
	CollisionRadius = radius;
	reachEndTime = 0;
	Velocity = Engine::Point(speed , 0);
	target = Engine::Point(PlayScene::EndGridPointx , static_cast<int>(floor(Position.y / PlayScene::BlockSize))) * PlayScene::BlockSize + Engine::Point(PlayScene::BlockSize / 2, PlayScene::BlockSize / 2);
}

void Enemy::Hit(float damage)
{
	hp -= damage;
	if (hp <= 0) {
		OnExplode();
		// Remove all turret's reference to target.
		for (auto& it: lockedTurrets)
			it->Target = nullptr;
		for (auto& it: lockedBullets)
			it->Target = nullptr;
		getPlayScene()->EarnMoney(money);
		getPlayScene()->EnemyGroup->RemoveObject(objectIterator);
		AudioHelper::PlayAudio("explosion.wav");
	}
}

void Enemy::Update(float deltaTime)
{
	PlayScene* scene = getPlayScene();
	float remainSpeed = speed * deltaTime;
	if (this->id == 1)
	{
		if ((Target && Position.x < Target->Position.x - PlayScene::BlockSize && Target->Position.y >= Position.y  && Target->Position.y < Position.y + scene->BlockSize) || !Target)
		{
			Position.x -= Velocity.x * deltaTime;
			Position.y += Velocity.y * deltaTime;
			if (Position.x <= PlayScene::EndGridPointx * PlayScene::BlockSize + PlayScene::BlockSize / 2)
			{
				Hit(hp);
				getPlayScene()->Hit();
				reachEndTime = 0;
				return;
			}
			Engine::Point vec = target - Position;
			reachEndTime = (vec.Magnitude() - remainSpeed) / speed;
		}

		if (Target)
		{
			//Engine::LOG(Engine::DEBUGGING) << "			b";
			// find closer targets
			float pos = Target->Position.x;
			for (auto& it : scene->TowerGroup->GetObjects())
			{
				//Engine::LOG(Engine::DEBUGGING) << "			a";
				if (Position.x <= it->Position.x && Position.x >= it->Position.x - (PlayScene::BlockSize * 4) && it->Position.y >= Position.y && it->Position.y < Position.y + scene->BlockSize && it->Position.x < pos)
				{
					Target = dynamic_cast<Turret*>(it);
					Target->lockedEnemies.push_back(this);
					lockedEnemyIterator = std::prev(Target->lockedEnemies.end());
					pos = Target->Position.x;
					//Engine::LOG(Engine::DEBUGGING) << "		target found";
					//break;
				}
			}
			if (Target->Position.x < Position.x && Target->Position.y >= Position.y  && Target->Position.y < Position.y + scene->BlockSize)
			{
				Target->lockedEnemies.erase(lockedEnemyIterator);
				Target = nullptr;
				lockedEnemyIterator = std::list<Enemy*>::iterator();
			}
			// Shoot reload.
			reload -= deltaTime;
			if (Target)
			{
				if (reload <= 0 && Position.x >= Target->Position.x - (PlayScene::BlockSize * 4))
				{
					// shoot.
					reload = coolDown;
					//Engine::LOG(Engine::DEBUGGING) << "		shoot";
					CreateBullet();
				}
			}
		}
		if (!Target)
		{
			// Lock first seen target.
			// Can be improved by Spatial Hash, Quad Tree, ...
			// However simply loop through all enemies is enough for this program.
			for (auto& it : scene->TowerGroup->GetObjects())
			{
				//Engine::LOG(Engine::DEBUGGING) << "			a";
				if (Position.x <= it->Position.x && Position.x >= it->Position.x - (PlayScene::BlockSize * 4) && it->Position.y >= Position.y && it->Position.y < Position.y + scene->BlockSize)
				{
					Target = dynamic_cast<Turret*>(it);
					Target->lockedEnemies.push_back(this);
					lockedEnemyIterator = std::prev(Target->lockedEnemies.end());
					//Engine::LOG(Engine::DEBUGGING) << "		target found";
					//break;
				}
			}
		}
		if (!Engine::Collider::IsRectOverlap(Position - Size / 2, Position + Size / 2, Engine::Point(0, 0), PlayScene::GetClientSize()))
			getPlayScene()->EnemyGroup->RemoveObject(objectIterator);
	}
	else if (this->id == 2)
	{
		if ((Target && Position.x > Target->Position.x + PlayScene::BlockSize && Target->Position.y >= Position.y  && Target->Position.y < Position.y + scene->BlockSize) || !Target)
		{
			Position.x -= Velocity.x * deltaTime;
			Position.y += Velocity.y * deltaTime;
			if (Position.x <= PlayScene::EndGridPointx * PlayScene::BlockSize + PlayScene::BlockSize / 2)
			{
				Hit(hp);
				getPlayScene()->Hit();
				reachEndTime = 0;
				return;
			}
			Engine::Point vec = target - Position;
			reachEndTime = (vec.Magnitude() - remainSpeed) / speed;
		}

		if (Target)
		{
			//Engine::LOG(Engine::DEBUGGING) << "			b";
			// find closer targets
			float pos = Target->Position.x;
			for (auto& it : scene->TowerGroup->GetObjects())
			{
				//Engine::LOG(Engine::DEBUGGING) << "			a";
				if (Position.x >= it->Position.x && Position.x <= it->Position.x + (PlayScene::BlockSize * 4) && it->Position.y >= Position.y && it->Position.y < Position.y + scene->BlockSize && it->Position.x > pos)
				{
					Target = dynamic_cast<Turret*>(it);
					Target->lockedEnemies.push_back(this);
					lockedEnemyIterator = std::prev(Target->lockedEnemies.end());
					pos = Target->Position.x;
					//Engine::LOG(Engine::DEBUGGING) << "		target found";
					//break;
				}
			}
			if (Target->Position.x > Position.x && Target->Position.y >= Position.y  && Target->Position.y < Position.y + scene->BlockSize)
			{
				Target->lockedEnemies.erase(lockedEnemyIterator);
				Target = nullptr;
				lockedEnemyIterator = std::list<Enemy*>::iterator();
			}
			// Shoot reload.
			reload -= deltaTime;
			if (Target)
			{
				if (reload <= 0 && Position.x <= Target->Position.x + (PlayScene::BlockSize * 4))
				{
					// shoot.
					reload = coolDown;
					//Engine::LOG(Engine::DEBUGGING) << "		shoot";
					CreateBullet();
				}
			}
		}
		if (!Target)
		{
			// Lock first seen target.
			// Can be improved by Spatial Hash, Quad Tree, ...
			// However simply loop through all enemies is enough for this program.
			for (auto& it : scene->TowerGroup->GetObjects())
			{
				//Engine::LOG(Engine::DEBUGGING) << "			a";
				if (Position.x >= it->Position.x && Position.x <= it->Position.x + (PlayScene::BlockSize * 4) && it->Position.y >= Position.y && it->Position.y < Position.y + scene->BlockSize)
				{
					Target = dynamic_cast<Turret*>(it);
					Target->lockedEnemies.push_back(this);
					lockedEnemyIterator = std::prev(Target->lockedEnemies.end());
					//Engine::LOG(Engine::DEBUGGING) << "		target found";
					//break;
				}
			}
		}
	}
	else
	{
		if ((Target && Position.x > Target->Position.x + PlayScene::BlockSize && Target->Position.y >= Position.y  && Target->Position.y < Position.y + scene->BlockSize) || !Target)
		{
			Position.x -= Velocity.x * deltaTime;
			Position.y += Velocity.y * deltaTime;
			if (Position.x <= PlayScene::EndGridPointx * PlayScene::BlockSize + PlayScene::BlockSize / 2)
			{
				Hit(hp);
				getPlayScene()->Hit();
				reachEndTime = 0;
				return;
			}
			Engine::Point vec = target - Position;
			reachEndTime = (vec.Magnitude() - remainSpeed) / speed;
		}

		if (Target)
		{
			//Engine::LOG(Engine::DEBUGGING) << "			b";
			// find closer targets
			float pos = Target->Position.x;
			for (auto& it : scene->TowerGroup->GetObjects())
			{
				//Engine::LOG(Engine::DEBUGGING) << "			a";
				if (Position.x >= it->Position.x && Position.x <= it->Position.x + (PlayScene::BlockSize * 4) && it->Position.y >= Position.y && it->Position.y < Position.y + scene->BlockSize && it->Position.x > pos)
				{
					Target = dynamic_cast<Turret*>(it);
					Target->lockedEnemies.push_back(this);
					lockedEnemyIterator = std::prev(Target->lockedEnemies.end());
					pos = Target->Position.x;
					//Engine::LOG(Engine::DEBUGGING) << "		target found";
					//break;
				}
			}
			if (Target->Position.x > Position.x && Target->Position.y >= Position.y  && Target->Position.y < Position.y + scene->BlockSize)
			{
				Target->lockedEnemies.erase(lockedEnemyIterator);
				Target = nullptr;
				lockedEnemyIterator = std::list<Enemy*>::iterator();
			}
			// Shoot reload.
			reload -= deltaTime;
			if (Target)
			{
				if (reload <= 0 && Position.x <= Target->Position.x + (PlayScene::BlockSize * 4))
				{
					// shoot.
					reload = coolDown;
					//Engine::LOG(Engine::DEBUGGING) << "		shoot";
					CreateBullet();
				}
			}
		}
		if (!Target)
		{
			// Lock first seen target.
			// Can be improved by Spatial Hash, Quad Tree, ...
			// However simply loop through all enemies is enough for this program.
			for (auto& it : scene->TowerGroup->GetObjects())
			{
				//Engine::LOG(Engine::DEBUGGING) << "			a";
				if (Position.x >= it->Position.x && Position.x <= it->Position.x + (PlayScene::BlockSize * 4) && it->Position.y >= Position.y && it->Position.y < Position.y + scene->BlockSize)
				{
					Target = dynamic_cast<Turret*>(it);
					Target->lockedEnemies.push_back(this);
					lockedEnemyIterator = std::prev(Target->lockedEnemies.end());
					//Engine::LOG(Engine::DEBUGGING) << "		target found";
					//break;
				}
			}
		}
	}
}

void Enemy::Draw() const
{
	Sprite::Draw();
	if (PlayScene::DebugMode) {
		// Draw collision radius.
		al_draw_circle(Position.x, Position.y, CollisionRadius, al_map_rgb(255, 0, 0), 2);
	}
}

/*
void Enemy::CreateBullet()
{
	Engine::Point diff = Engine::Point(1, 0);
	float rotation = ALLEGRO_PI / 2;
	getPlayScene()->BulletGroup->AddNewObject(new EnemyBullet(Position, diff, rotation, this));
	AudioHelper::PlayAudio("missile.wav");
}
*/