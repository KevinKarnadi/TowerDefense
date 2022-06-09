#ifndef BOMBBUTTON_HPP
#define BOMBBUTTON_HPP
#include <string>

#include "ImageButton.hpp"
#include "Sprite.hpp"

class PlayScene;

class BombButton : public Engine::ImageButton
{
protected:
	PlayScene* getPlayScene();
public:
	int money;
	// Engine::Sprite Base;
	Engine::Sprite Bomb;
	BombButton(std::string img, std::string imgIn, Engine::Sprite Bomb, float x, float y, int money);
	void Update(float deltaTime) override;
	void Draw() const override;
};
#endif // BOMBBUTTON_HPP
