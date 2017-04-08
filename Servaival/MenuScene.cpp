#include "MenuScene.h"

#include "GameScene.h"




MenuScene::MenuScene(const caUtil::ResourcePool& resPool)
	: m_pool(resPool)
{

}


MenuScene::~MenuScene()
{

}

//########################################################################################################

void MenuScene::onInitialize(caDraw::Window& owner)
{
	auto winSize = owner.getSize();


	m_font = m_pool.getFont("default");

	m_texTitle = m_pool.getTexture("title");
	m_texToStart = m_pool.getTexture("to_start");

	m_sndBgm = m_pool.getSound("bgm_light");
	m_sndBgm->play(true);


	m_panel = caFactory->createPanel();
	m_panel->size = static_cast<caDraw::SizeF>(winSize);
	m_panel->transform.position = { 0, 0 };


	addPanel(m_panel);
}


void MenuScene::onRelease()
{
	m_sndBgm->stop();
}


void MenuScene::onUpdate(caDraw::Window& owner)
{
	if (caKeyboard->isKeyDown(caSys::Keys::Escape))
	{
		reserveNextScene(nullptr);
	}
	else if (caKeyboard->isKeyDown(caSys::Keys::Space))
	{
		m_pool.getSound("hoowhip")->play();


		reserveNextScene<GameScene>(m_pool);
	}
}


void MenuScene::onDrawBack(caDraw::Graphics& g)
{
	caDraw::PointF center = { m_panel->size.width / 2, m_panel->size.height / 2 };


	auto& textureArt = g.textureArtist;


	g.rectangleArtist->beginFillRectangle();

	g.rectangleArtist->fillRectangle({ 0, 0 }, m_panel->size, caDraw::Color(239, 228, 176));

	g.rectangleArtist->endFillRectangle();


	textureArt->beginDrawTexture();

	textureArt->drawTexture(*m_texTitle, center + caDraw::PointF(-458.5f, -150.0f));
	textureArt->drawTexture(*m_texToStart, center + caDraw::PointF(-209.0f, 128.0f));

	textureArt->endDrawTexture();
}


void MenuScene::onDrawFront(caDraw::Graphics& g)
{

}

