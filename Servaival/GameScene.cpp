#include "GameScene.h"

#include "RankScene.h"

#include <random>
#include <algorithm>




GameScene::GameScene(const caUtil::ResourcePool& resPool)
	: m_pool(resPool)

	, m_gameStarted(false)
	, m_gameOver(false)
	, m_score(0)
	, m_grass(36)
	, m_maxLength(1)
	, m_dirtOffset(0)
	, m_noiseGage(-1)

	, m_enemyGage(120)
{
	std::srand(std::random_device()());
}


GameScene::~GameScene()
{

}

//########################################################################################################

void GameScene::onInitialize(caDraw::Window& owner)
{
	auto winSize = owner.getSize();


	loadResources(winSize);

	m_sndBgmHeart->play(true);
	m_sndBgmEngine->play(true);


	m_panel = caFactory->createPanel();
	m_panel->size = static_cast<caDraw::SizeF>(winSize);
	m_panel->transform.position = { 0, 0 };

	addPanel(m_panel);


	initializeGame(winSize);
}


void GameScene::onRelease()
{
	m_sndBgmHeart->stop();
	m_sndBgmEngine->stop();
	m_sndBgmRunning->stop();
	m_sndPak->stop();
	m_sndHit->stop();
}


void GameScene::onUpdate(caDraw::Window& owner)
{
	if (m_gameOver)
	{
		updateGameOver();
	}
	else
	{
		// ���ӿ��� ���°� �ƴϸ�.


		m_bus.update();
		m_serval.update();


		removeDeadObstacles();


		/// ������ ���� ������ �Ÿ�.
		const float leftDistance = m_serval.transform.position.x - (m_bus.transform.position.x + 251);


		for (auto& grass : m_grass)
		{
			grass->update();
		}


		// �÷��̾ �޸��� ���̸�
		if (caKeyboard->isKeyDown(caSys::Keys::Right) || caKeyboard->isKeyPressed(caSys::Keys::Right))
		{
			// ������ ���� ���۵��� �ʾ�����
			if (m_gameStarted == false)
			{
				// ����
				m_gameStarted = true;

				updateWhenFirstRun();
			}


			// ���� ����
			++m_score;


			moveGrasses();

			moveGround();


			updateEnemySpawner();

			updateEnemyAndCheckHit();
		}
		else if (m_gameStarted)
		{
			// �÷��̾ �޸��� ���� �ƴϰ� ������ ���۵Ǿ�����

			moveBus();
		}


		if (leftDistance <= 0.0f)
		{
			m_gameOver = true;

			whenGameOver();
		}


		if (caKeyboard->isKeyDown(caSys::Keys::Space))
		{
			jumpServal();
		}


		updateEngineVolume(leftDistance);


		for (auto& effect : m_effects)
		{
			effect->update();
		}

		removeEndEffect();
	}
}


void GameScene::onDrawBack(caDraw::Graphics& g)
{
	auto& texArt = g.textureArtist;
	auto& rectArt = g.rectangleArtist;


	// ��� �ϴ�.
	m_texSky->draw(g, m_panel->transform);


	rectArt->beginFillRectangle();

	// �� ���� �� ƴ���� �ϴ��� ���̴� ���� ����.
	rectArt->fillRectangle(caDraw::RectangleF(0, m_panel->size.height / 2 + 64,
		m_panel->size.width, m_panel->size.height / 2), caDraw::Color::Black);

	rectArt->endFillRectangle();


	// �ܵ�.
	for (auto& grass : m_grass)
	{
		grass->draw(g, m_panel->transform);
	}


	// ����.
	m_serval.draw(g, m_panel->transform);


	// ��ֹ�.
	for (auto& enemy : m_obstacles)
	{
		enemy->draw(g, m_panel->transform);
	}

	
	// ����.
	m_bus.draw(g, m_panel->transform);


	// ����Ʈ.
	for (auto& effect : m_effects)
	{
		effect->draw(g, m_panel->transform);
	}


	texArt->beginDrawTexture();

	// ��.
	for (float x = -16; x < m_panel->size.width + 200.0f; x += 64.0f)
	{
		// ���� ���� �ܵ� �ִ� ��.
		texArt->drawTexture(*m_texDirtGrass, 
			caDraw::PointF(x + m_dirtOffset, m_panel->size.height / 2 + 32));

		// �� �Ʒ��� ��.
		for (float y = 0; y < 5 * 64.0f; y += 64.0f)
		{
			texArt->drawTexture(*m_texDirt,
				caDraw::PointF(x + m_dirtOffset, m_panel->size.height / 2 + 32 + 64 + y));
		}
	}

	texArt->endDrawTexture();


	if (m_gameOver)
	{
		texArt->beginDrawTexture();

		// ������ ������ ġ�� ȿ��.
		texArt->drawTexture(*m_texBlood0, m_serval.transform.position + caDraw::PointF(-60, 0));
		texArt->drawTexture(*m_texBlood1, m_serval.transform.position + caDraw::PointF(-30, 10));
		texArt->drawTexture(*m_texBlood0, m_serval.transform.position + caDraw::PointF(-20, 20));
		texArt->drawTexture(*m_texBlood1, m_serval.transform.position + caDraw::PointF(-65, 5));
		texArt->drawTexture(*m_texBlood0, m_serval.transform.position + caDraw::PointF(-42, 8));

		texArt->drawTexture(*m_texBlood1, m_serval.transform.position + caDraw::PointF(-105, -16));
		texArt->drawTexture(*m_texBlood0, m_serval.transform.position + caDraw::PointF(-82, 0));

		texArt->endDrawTexture();
	}
}


void GameScene::onDrawFront(caDraw::Graphics& g)
{
	auto& textArt = g.textArtist;


	textArt->beginDrawString(*m_font);

	textArt->drawString("SCORE : " + std::to_string(m_score),
		caDraw::PointF(4, 4), caDraw::Color(200, 34, 177, 76));

	textArt->endDrawString();


	if (m_gameOver)
	{
		int bakAlpha = m_texGameover->color.a;

		if (bakAlpha < 0)
			m_texGameover->color.a = 0;
		else if (bakAlpha > 255)
			m_texGameover->color.a = 255;

		m_texGameover->draw(g, m_panel->transform);


		int bakCharSize = m_font->getCharacterSize();
		m_font->setCharacterSize(64);

		textArt->beginDrawString(*m_font);

		textArt->drawString("SCORE\n" + std::to_string(m_score),
			caDraw::PointF(m_panel->size.width / 2, m_panel->size.height / 2),
			caDraw::Color(m_texGameover->color.a, 255, 49, 38),
			caDraw::TextAligns::Center);

		textArt->endDrawString();

		m_font->setCharacterSize(bakCharSize);


		m_texGameover->color.a = bakAlpha;
	}


	if (m_noiseGage >= 0 && m_noiseGage % 4 != 0)
	{
		m_texNoise->draw(g, m_panel->transform);
	}
}

//########################################################################################################

void GameScene::loadResources(const caDraw::Size& winSize)
{
	m_font = m_pool.getFont("default");

	m_texSky = canew<caDraw::DrawableTexture>();
	m_texSky->setTexture(m_pool.getTexture("sky"));
	m_texSky->transform.scale = { winSize.width / 644.0f, winSize.height / 364.0f };
	m_texGameover = canew<caDraw::DrawableTexture>();
	m_texGameover->setTexture(m_pool.getTexture("gameover"));
	m_texGameover->transform.scale = { winSize.width / 644.0f, winSize.height / 363.0f };
	m_texGameover->color.a = -300;
	m_texNoise = canew<caDraw::DrawableTexture>();
	m_texNoise->setTexture(m_pool.getTexture("noise"));
	m_texNoise->transform.scale = { winSize.width / 324.0f, winSize.height / 184.0f };
	m_texNoise->color.a = 180;
	m_texBlood0 = m_pool.getTexture("blood0");
	m_texBlood1 = m_pool.getTexture("blood1");
	m_texDirt = m_pool.getTexture("dirt");
	m_texDirtGrass = m_pool.getTexture("dirt_grass");

	m_sndBgmHeart = m_pool.getSound("bgm_heart");
	m_sndBgmGameover = m_pool.getSound("bgm_heart2");
	m_sndBgmEngine = m_pool.getSound("bgm_engine");
	m_sndBgmEngine->setVolume(1);
	m_sndBgmRunning = m_pool.getSound((std::random_device()() % 2 == 0) ? "bgm_adv" : "bgm_kg2");
	m_sndPak = m_pool.getSound("pak");
	m_sndHit = m_pool.getSound("hit");
	m_sndJump = m_pool.getSound("jump");


	m_bus.load(m_pool.getTexture("bus"), m_pool.getTexture("bus_wheel"));
	m_serval.load(m_pool.getTexture("serval"));
}


void GameScene::initializeGame(const caDraw::Size& winSize)
{
	m_bus.transform.position = { 16, static_cast<float>(winSize.height / 2 - 64) };

	m_serval.transform.position = {
		static_cast<float>(winSize.width - 256),
		static_cast<float>(winSize.height / 2 - 33)
	};

	float grassX = -4;
	for (auto& grass : m_grass)
	{
		grass = std::make_unique<Grass>();
		grass->load(m_pool);
		grass->transform.position = { grassX, static_cast<float>(winSize.height / 2 - 16) };

		grassX += 33.2f;
	}

	m_maxLength = m_serval.transform.position.x - (m_bus.transform.position.x + 251);
}

//########################################################################################################

void GameScene::jumpServal()
{
	if (m_serval.canJump())
	{
		m_serval.jump();


		m_sndJump->stop();
		m_sndJump->play();
	}
}


void GameScene::updateGameOver()
{
	m_texGameover->color.a += 1;

	if (m_texGameover->color.a > 400)
	{
		// ���� ����.
		reserveNextScene<RankScene>(m_pool, m_score);
	}


	if (m_noiseGage >= 0)
	{
		++m_noiseGage;

		if (m_noiseGage > 10)
		{
			m_noiseGage = -1;
		}
	}
}


void GameScene::removeDeadObstacles()
{
	auto itrBeginRemove = std::remove_if(m_obstacles.begin(), m_obstacles.end(),
		[&sndPak = m_sndPak, &bus = m_bus, &effects = m_effects,
		&texBlood0 = m_texBlood0, &texBlood1 = m_texBlood1](const auto& enemy)
	{
		// ������ ġ���ų� �ٸ� ������ �׾�����
		if (enemy->transform.position.x < bus.transform.position.x + 221
			|| enemy->isDead())
		{
			// ����Ʈ�� �߻���Ų��.
			for (int i = 0; i < 6; ++i)
			{
				caDraw::VectorF speed = {
					static_cast<float>(4 + std::rand() % 12),
					static_cast<float>(2 - std::rand() % 16),
				};

				auto newEffect = std::make_unique<Effect>(speed);
				newEffect->load((std::random_device()() % 2 == 0) ? texBlood0 : texBlood1);
				newEffect->transform.position = enemy->transform.position;

				effects.emplace_back(std::move(newEffect));
			}


			// �΋H���� �Ҹ� ���.
			sndPak->stop();
			sndPak->play();


			return true;
		}


		return false;
	});
	m_obstacles.erase(itrBeginRemove, m_obstacles.end());
}


void GameScene::updateWhenFirstRun()
{
	m_sndBgmHeart->stop();
	m_sndBgmRunning->play(true);
}


void GameScene::moveGrasses()
{
	for (auto& grass : m_grass)
	{
		grass->transform.position.move(-6.0f, 0);
	}

	// �������� ȭ���� ��� ���� ���� ������ �̵�.
	if (m_grass[0]->transform.position.x < -64)
	{
		m_grass[0]->transform.position = m_grass.back()->transform.position;
		m_grass[0]->transform.position.move(33.2f, 0);

		auto popGrass = std::move(m_grass[0]);
		m_grass.erase(m_grass.begin());

		m_grass.emplace_back(std::move(popGrass));
	}
}


void GameScene::moveGround()
{
	m_dirtOffset -= 6.0f;

	if (m_dirtOffset < -64.0f)
	{
		m_dirtOffset += 64.0f;
	}
}


void GameScene::updateEnemySpawner()
{
	if (m_enemyGage <= 0)
	{
		m_enemyGage = (rand() % 256) + 15;


		auto newEnemy = std::make_unique<Obstacle>();
		newEnemy->load(m_pool.getTexture("obstacle" + std::to_string(std::random_device()() % 2)));
		newEnemy->transform.position = {
			static_cast<float>(m_panel->size.width + 64),
			static_cast<float>(m_panel->size.height / 2 + 2)
		};

		m_obstacles.emplace_back(std::move(newEnemy));
	}
	else
	{
		--m_enemyGage;
	}
}


void GameScene::updateEnemyAndCheckHit()
{
	for (auto& enemy : m_obstacles)
	{
		// Hit Test.

		caDraw::VectorF serval = {
			m_serval.transform.position.x - 22,
			m_serval.transform.position.y + 48
		};
		caDraw::VectorF obs = {
			enemy->transform.position.x + 8,
			enemy->transform.position.y,
		};

		auto subVec = serval - obs;

		if (subVec.getLengthSq() < 16 * 16)
		{
			enemy->die();

			m_serval.hit();

			whenHit();
		}


		// Update Enemy.
		enemy->update();

		// Move Enemy.
		enemy->transform.position.move(-6.0f, 0);
	}
}


void GameScene::moveBus()
{
	m_bus.transform.position.move(6.0f, 0.0f);
}


void GameScene::whenGameOver()
{
	m_pool.getSound("noise")->play();
	m_pool.getSound("dook")->play();
	m_pool.getSound("blood")->play();

	m_sndBgmRunning->stop();
	m_sndBgmEngine->stop();
	m_sndBgmGameover->play(true);


	// ��� �ϴ��� ������ ����.
	m_texSky->color.g = 0;
	m_texSky->color.b = 0;


	// �� �̹����� ��ü.
	m_texDirt = m_pool.getTexture("blood_dirt");
	m_texDirtGrass = m_pool.getTexture("blood_dirt_grass");


	// ������ �̹����� ������ ������� ��ü
	m_serval.load(m_pool.getTexture("dead_serval"));

	// ���� �����߿� �׾��ٸ� ���� �� Y ��ǥ�� ����.
	m_serval.restoreYToBeforeJump();

	// �̹��� �������� ���� ��ġ ����.
	m_serval.transform.position.move(64, 40);


	m_noiseGage = 0;
}


void GameScene::updateEngineVolume(float leftDistance)
{
	// �Ÿ��� �������� �Ҹ��� ũ�� ����.

	float engineVolume = 100.0f - leftDistance / m_maxLength * 100.0f;
	if (engineVolume < 1)
		engineVolume = 1;
	m_sndBgmEngine->setVolume(engineVolume);
}


void GameScene::removeEndEffect()
{
	auto effectItrBeginRemove = std::remove_if(m_effects.begin(), m_effects.end(),
		[&panel = m_panel](const auto& effect)
	{
		// ȭ�� �Ʒ� ������ ��� ����Ʈ.
		return (effect->transform.position.y > panel->size.height + 128);
	});
	m_effects.erase(effectItrBeginRemove, m_effects.end());
}


void GameScene::whenHit()
{
	m_sndHit->stop();
	m_sndHit->play();
}

