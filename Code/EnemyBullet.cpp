#include <random>

#include "EnemyBullet.hpp"
#include "Collider.hpp"
#include "Turret.hpp"
#include "GameEngine.hpp"
#include "Group.hpp"
#include "IObject.hpp"
#include "IScene.hpp"
#include "PlayScene.hpp"
#include "Point.hpp"
#include "Sprite.hpp"

#include "DirtyEffect.hpp"
#include "NormalEnemy.hpp"
#include "SofaEnemy.hpp"
#include "StrongEnemy.hpp"
#include "HeadEnemy.hpp"
#include "IceEnemy.hpp"

PlayScene* EnemyBullet::getPlayScene()
{
	return dynamic_cast<PlayScene*>(Engine::GameEngine::GetInstance().GetActiveScene());
}

void EnemyBullet::OnExplode(Turret* enemy)
{
	std::random_device dev;
	std::mt19937 rng(dev());
	std::uniform_int_distribution<std::mt19937::result_type> dist(2, 5);
	getPlayScene()->GroundEffectGroup->AddNewObject(new DirtyEffect("play/dirty-3.png", dist(rng), enemy->Position.x, enemy->Position.y));
}

EnemyBullet::EnemyBullet(std::string img, float speed, float damage, Engine::Point position, Engine::Point forwardDirection, float rotation, Enemy* parent) :
	Sprite(img, position.x, position.y), speed(speed), damage(damage), parent(parent)
{
	Velocity = forwardDirection * speed;
	Rotation = rotation;
	CollisionRadius = 4;
}

void EnemyBullet::Update(float deltaTime)
{
	Sprite::Update(deltaTime);
	PlayScene* scene = getPlayScene();
	// Can be improved by Spatial Hash, Quad Tree, ...
	// However simply loop through all enemies is enough for this program.
	for (auto& it : scene->TowerGroup->GetObjects())
	{
		Turret* enemy = dynamic_cast<Turret*>(it);
		if (!enemy->Visible)
			continue;
		if (Engine::Collider::IsCircleOverlap(Position, CollisionRadius, enemy->Position, enemy->CollisionRadius))
		{
			OnExplode(enemy);
			if (parent->id == 2)
				enemy->Freeze();
			enemy->Hit(damage);
			getPlayScene()->BulletGroup->RemoveObject(objectIterator);
			return;
		}
	}
	// Check if out of boundary.
	if (!Engine::Collider::IsRectOverlap(Position - Size / 2, Position + Size / 2, Engine::Point(0, 0), PlayScene::GetClientSize()))
		getPlayScene()->BulletGroup->RemoveObject(objectIterator);
}
