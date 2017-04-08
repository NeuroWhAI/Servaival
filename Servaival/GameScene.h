#ifndef __SERVAIVAL__GAME_SCENE_H__
#define __SERVAIVAL__GAME_SCENE_H__


#include <deque>
#include <memory>
#include <vector>

#include <CodeAdapter\EasyCA.h>

#include "Bus.h"
#include "Serval.h"
#include "Grass.h"
#include "Obstacle.h"
#include "Effect.h"




class GameScene : public caUtil::Scene
{
public:
	explicit GameScene(const caUtil::ResourcePool& resPool);
	virtual ~GameScene();


private:
	caUtil::ResourcePool m_pool;
	caDraw::FontPtr m_font;
	caDraw::DrawableTexturePtr m_texSky;
	caDraw::DrawableTexturePtr m_texGameover;
	caDraw::DrawableTexturePtr m_texNoise;
	caDraw::TexturePtr m_texDirt;
	caDraw::TexturePtr m_texDirtGrass;
	caDraw::TexturePtr m_texBlood0;
	caDraw::TexturePtr m_texBlood1;
	caAudio::SoundPtr m_sndBgmHeart;
	caAudio::SoundPtr m_sndBgmGameover;
	caAudio::SoundPtr m_sndBgmEngine;
	caAudio::SoundPtr m_sndBgmRunning;
	caAudio::SoundPtr m_sndPak;
	caAudio::SoundPtr m_sndHit;
	caAudio::SoundPtr m_sndJump;


private:
	caDraw::PanelPtr m_panel;


private:
	bool m_gameStarted;
	bool m_gameOver;
	std::size_t m_score;
	Bus m_bus;
	Serval m_serval;
	std::deque<std::unique_ptr<Grass>> m_grass;
	float m_maxLength;
	float m_dirtOffset;
	int m_noiseGage;


private:
	std::vector<std::unique_ptr<Obstacle>> m_obstacles;
	int m_enemyGage;


private:
	std::vector<std::unique_ptr<Effect>> m_effects;


public:
	virtual void onInitialize(caDraw::Window& owner) override;
	virtual void onRelease() override;
	virtual void onUpdate(caDraw::Window& owner) override;
	virtual void onDrawBack(caDraw::Graphics& g) override;
	virtual void onDrawFront(caDraw::Graphics& g) override;


private:
	void loadResources(const caDraw::Size& winSize);
	void initializeGame(const caDraw::Size& winSize);


private:
	void jumpServal();
	void updateGameOver();
	void removeDeadObstacles();
	void updateWhenFirstRun();
	void moveGrasses();
	void moveGround();
	void updateEnemySpawner();
	void updateEnemyAndCheckHit();
	void moveBus();
	void whenGameOver();
	void updateEngineVolume(float leftDistance);
	void removeEndEffect();
	void whenHit();
};


#endif