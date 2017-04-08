#include "LoadScene.h"

#include "MenuScene.h"




LoadScene::LoadScene()
	: m_loadInfo("Res/Img/info.txt")
	, m_loadFileName("...")
	, m_loadStage(0)
	, m_loadGage(0)
{

}


LoadScene::~LoadScene()
{

}

//########################################################################################################

void LoadScene::onInitialize(caDraw::Window& owner)
{
	auto winSize = owner.getSize();


	m_font = m_pool.createFont("default");
	m_font->loadFromFile("Res/Font/GN-KillGothic-U-KanaNA.ttf");
	m_font->setCharacterSize(32);


	m_panel = caFactory->createPanel();
	m_panel->size = static_cast<caDraw::SizeF>(winSize);
	m_panel->transform.position = { 0, 0 };


	addPanel(m_panel);
}


void LoadScene::onRelease()
{

}


void LoadScene::onUpdate(caDraw::Window& owner)
{
	if (m_loadGage <= 0)
	{
		m_loadGage = 2;


		if (m_loadInfo.is_open() == false || m_loadInfo.eof() || m_loadInfo.bad())
		{
			if (m_loadStage < 1)
			{
				++m_loadStage;

				m_loadInfo.close();
				m_loadInfo.clear();

				if (m_loadStage == 1)
				{
					m_loadInfo.open("Res/Sound/info.txt");
				}
			}
			else
			{
				reserveNextScene<MenuScene>(m_pool);
			}
		}
		else
		{
			char name[1024], path[1024];

			m_loadInfo.getline(name, 1024);
			m_loadInfo.getline(path, 1024);

			m_loadFileName = name;
			if (m_loadFileName.length() > 0)
			{
				if (m_loadStage == 0)
				{
					auto tex = m_pool.createTexture(name);
					tex->loadFromFile(path);
				}
				else if (m_loadStage == 1)
				{
					auto snd = m_pool.createSound(name);
					snd->loadFromFile(path);
				}
			}
			else
			{
				m_loadInfo.close();
				m_loadInfo.clear();
			}
		}
	}
	else
	{
		--m_loadGage;
	}
}


void LoadScene::onDrawBack(caDraw::Graphics& g)
{
	caDraw::PointF center = { m_panel->size.width / 2, m_panel->size.height / 2 };


	g.textArtist->beginDrawString(*m_font);

	g.textArtist->drawString("Loading...", center - caDraw::PointF(0, 64), caDraw::Color::White,
		caDraw::TextAligns::Center);

	g.textArtist->drawString(m_loadFileName, center, caDraw::Color::White,
		caDraw::TextAligns::Center);

	g.textArtist->endDrawString();
}


void LoadScene::onDrawFront(caDraw::Graphics& g)
{

}

