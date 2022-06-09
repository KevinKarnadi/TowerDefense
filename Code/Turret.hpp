#ifndef TURRET_HPP
#define TURRET_HPP
#include <allegro5/base.h>
#include <list>
#include <string>

#include "Sprite.hpp"

#include "Point.hpp"

class Enemy;
class PlayScene;

class Turret: public Engine::Sprite
{
protected:
    int price;
    float coolDown;
    float reload = 0;
    float rotateRadian = 2 * ALLEGRO_PI;
    std::list<Turret*>::iterator lockedTurretIterator;
    PlayScene* getPlayScene();
    // Reference: Design Patterns - Factory Method.
    virtual void CreateBullet() = 0;

	float hp;

public:
    bool Enabled = true;
    bool Preview = false;
    Enemy* Target = nullptr;
    Turret(/*std::string imgBase,*/std::string imgTurret, float x, float y,/* float radius,*/ int price, float coolDown, float hp);
    void Update(float deltaTime) override;
    void Draw() const override;
	int GetPrice() const;

	std::list<Enemy*> lockedEnemies;
	void Hit(float damage);
	virtual void OnExplode();
	void Freeze();
	bool Frozen = false;
	double frozen_time;
};
#endif // TURRET_HPP
