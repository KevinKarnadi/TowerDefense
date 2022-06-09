#ifndef BULLET_HPP
#define BULLET_HPP
#include <string>

#include "Sprite.hpp"

class Enemy;
class PlayScene;
class Turret;
namespace Engine
{
	struct Point;
}  // namespace Engine

class EnemyBullet : public Engine::Sprite
{
protected:
	float speed;
	float damage;
	Enemy* parent;
	PlayScene* getPlayScene();
	virtual void OnExplode(Turret* enemy);
public:
	Enemy* Target = nullptr;
	explicit EnemyBullet(std::string img, float speed, float damage, Engine::Point position, Engine::Point forwardDirection, float rotation, Enemy* parent);
	void Update(float deltaTime) override;

	int id;
};
#endif // BULLET_HPP
