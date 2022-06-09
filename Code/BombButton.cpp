#include <allegro5/color.h>

#include "GameEngine.hpp"
#include "IScene.hpp"
#include "PlayScene.hpp"
#include "BombButton.hpp"

PlayScene* BombButton::getPlayScene()
{
	return dynamic_cast<PlayScene*>(Engine::GameEngine::GetInstance().GetActiveScene());
}

BombButton::BombButton(std::string img, std::string imgIn, Engine::Sprite Bomb, float x, float y, int money) :
	ImageButton(img, imgIn, x, y), money(money), Bomb(Bomb)
{
}

void BombButton::Update(float deltaTime)
{
	ImageButton::Update(deltaTime);
	if (getPlayScene()->GetMoney() >= money)
	{
		Enabled = true;
		Bomb.Tint = al_map_rgba(255, 255, 255, 255);
	}
	else
	{
		Enabled = false;
		Bomb.Tint = al_map_rgba(0, 0, 0, 160);
	}
}

void BombButton::Draw() const
{
	ImageButton::Draw();
	Bomb.Draw();
}
