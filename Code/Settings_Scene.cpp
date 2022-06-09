#include <functional>
#include <string>

#include "AudioHelper.hpp"
#include "GameEngine.hpp"
#include "Image.hpp"
#include "ImageButton.hpp"
#include "Label.hpp"
#include "Settings_Scene.hpp"
#include "PlayScene.hpp"
#include "Point.hpp"
#include "Slider.hpp"
#include "StageSelectScene.hpp"

void SettingsScene::Initialize()
{
	int w = Engine::GameEngine::GetInstance().GetScreenSize().x;
	int h = Engine::GameEngine::GetInstance().GetScreenSize().y;
	int halfW = w / 2;
	int halfH = h / 2;
	Engine::ImageButton* btn;

	btn = new Engine::ImageButton("stage-select/dirt.png", "stage-select/floor.png", w - 350, h - 150, 300, 100);
	btn->SetOnClickCallback(std::bind(&SettingsScene::BackOnClick, this));
	AddNewControlObject(btn);
	AddNewObject(new Engine::Label("Back", "pirulen.ttf", 36, w - 200, h - 100, 0, 0, 0, 255, 0.5, 0.5));

	// TODO 1 (3/3): Move the sliderBGM, sliderSFX to the setting Scene and make sure the background music will be played
	Slider *sliderBGM, *sliderSFX;
	sliderBGM = new Slider(40 + halfW - 95, halfH - 50 - 2, 190, 4);
	sliderBGM->SetOnValueChangedCallback(std::bind(&SettingsScene::BGMSlideOnValueChanged, this, std::placeholders::_1));
	AddNewControlObject(sliderBGM);
	AddNewObject(new Engine::Label("BGM: ", "pirulen.ttf", 28, 40 + halfW - 60 - 95, halfH - 50, 255, 255, 255, 255, 0.5, 0.5));
	sliderSFX = new Slider(40 + halfW - 95, halfH + 50 - 2, 190, 4);
	sliderSFX->SetOnValueChangedCallback(std::bind(&SettingsScene::SFXSlideOnValueChanged, this, std::placeholders::_1));
	AddNewControlObject(sliderSFX);
	AddNewObject(new Engine::Label("SFX: ", "pirulen.ttf", 28, 40 + halfW - 60 - 95, halfH + 50, 255, 255, 255, 255, 0.5, 0.5));
	// Not safe if release resource while playing, however we only free while change scene, so it's fine.
	bgmInstance = AudioHelper::PlaySample("select.ogg", true, AudioHelper::BGMVolume);
	//AudioHelper::StopSample(bgmInstance);
	sliderBGM->SetValue(AudioHelper::BGMVolume);
	sliderSFX->SetValue(AudioHelper::SFXVolume);
}

void SettingsScene::Terminate()
{
	AudioHelper::StopSample(bgmInstance);
	bgmInstance = std::shared_ptr<ALLEGRO_SAMPLE_INSTANCE>();
	IScene::Terminate();
}

void SettingsScene::BackOnClick()
{
	Engine::GameEngine::GetInstance().ChangeScene("stage-select");
}

void SettingsScene::BGMSlideOnValueChanged(float value)
{
	AudioHelper::ChangeSampleVolume(bgmInstance, value);
	AudioHelper::BGMVolume = value;
}

void SettingsScene::SFXSlideOnValueChanged(float value)
{
	AudioHelper::SFXVolume = value;
}