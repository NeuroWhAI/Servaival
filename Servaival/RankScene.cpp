#include "RankScene.h"

#include <fstream>
#include <Windows.h>

#include "MenuScene.h"




RankScene::RankScene(const caUtil::ResourcePool& resPool, std::size_t score)
	: m_pool(resPool)

	, m_score(score)

	, m_rankLoading(false)
	, m_rankLoaded(false)
{

}


RankScene::~RankScene()
{

}

//########################################################################################################

void RankScene::onInitialize(caDraw::Window& owner)
{
	auto winSize = owner.getSize();


	m_font = m_pool.getFont("default");

	m_texGameover = canew<caDraw::DrawableTexture>();
	m_texGameover->setTexture(m_pool.getTexture("gameover"));
	m_texGameover->transform.scale = { winSize.width / 644.0f, winSize.height / 363.0f };


	m_panel = caFactory->createPanel();
	m_panel->size = static_cast<caDraw::SizeF>(winSize);
	m_panel->transform.position = { 0, 0 };

	m_boxName = canew<caUI::TextBox>();
	m_boxName->setFont(m_font);
	m_boxName->setSize({ 300, 32 });
	m_boxName->transform.position = { winSize.width / 2.0f - 150, winSize.height / 2.0f + 16.0f };
	m_boxName->setForeColor(caDraw::Color(255, 49, 38));
	m_boxName->setBackColor(caDraw::Color(150, 255, 255, 255));
	m_boxName->WhenTextChanged = [&me = m_boxName](const caUI::EventArgs& args)
	{
		auto text = static_cast<std::wstring>(me->getText());

		for (auto& ch : text)
		{
			if (ch == L' ' || ch == L'\t')
			{
				ch = L'-';
			}
			else if (!((ch >= L'a' && ch <= L'z')
				|| (ch >= L'A' && ch <= L'Z')
				|| (ch >= L'0' && ch <= L'9')))
			{
				ch = L'-';
			}
		}

		if (text.length() > 14)
		{
			text.erase(text.end() - 1);
		}

		me->setText(text);
	};


	m_panel->addControl(m_boxName);


	addPanel(m_panel);
}


void RankScene::onRelease()
{
	m_pool.getSound("bgm_heart2")->stop();


	if (m_rankLoading)
	{
		m_taskLoadRanking.wait();

		m_rankLoading = false;
	}
}


void RankScene::onUpdate(caDraw::Window& owner)
{
	if (caKeyboard->isKeyDown(caSys::Keys::Escape)
		|| (m_rankLoaded
			&& (caKeyboard->isKeyDown(caSys::Keys::Return)
			|| caKeyboard->isKeyDown(caSys::Keys::Space))))
	{
		reserveNextScene<MenuScene>(m_pool);
	}
	else if (caKeyboard->isKeyDown(caSys::Keys::Return)
		&& m_rankLoading == false && m_rankLoaded == false)
	{
		auto name = static_cast<std::string>(m_boxName->getText());

		if (name.length() > 0)
		{
			m_boxName->setEnabled(false);
			m_boxName->setVisible(false);

			m_rankLoading = true;


			m_taskLoadRanking = std::async(std::launch::async,
				[name, score = m_score, &ranks = m_rankList,
				&onLoading = m_rankLoading, &finished = m_rankLoaded]
			{
				char buf[FILENAME_MAX];
				GetCurrentDirectoryA(FILENAME_MAX, buf);

				std::string cmd = std::string(buf) + "/Res/RankManager.exe NeuroWhAI \""
					+ buf
					+ "/Res\" DB_URI \""
					+ name + "\" " + std::to_string(score);

				std::system(cmd.c_str());


				ranks.clear();

				std::ifstream fr("Res/rank.txt");

				if (fr.is_open())
				{
					try
					{
						int count = 0;
						fr >> count;

						for (int i = 0; i < count; ++i)
						{
							RankingData rank;
							fr >> rank.name;
							fr >> rank.score;

							ranks.emplace_back(std::move(rank));
						}
					}
					catch (std::exception)
					{
						// TODO: 
					}

					fr.close();
				}


				onLoading = false;
				finished = true;
			});
		}
	}
}


void RankScene::onDrawBack(caDraw::Graphics& g)
{
	auto& textArt = g.textArtist;


	m_texGameover->draw(g, m_panel->transform);


	if (m_rankLoaded)
	{
		textArt->beginDrawString(*m_font);

		std::size_t rank = 0;

		for (auto& data : m_rankList)
		{
			float yPos = 100.0f + static_cast<float>(rank) * 64.0f;

			++rank;

			textArt->drawString("#" + std::to_string(rank),
				caDraw::PointF(m_panel->size.width / 2 - 300, yPos),
				caDraw::Color(240, 240, 240));

			textArt->drawString(data.name,
				caDraw::PointF(m_panel->size.width / 2 - 40, yPos),
				caDraw::Color(240, 240, 240),
				caDraw::TextAligns::Bottom);

			textArt->drawString(std::to_string(data.score),
				caDraw::PointF(m_panel->size.width / 2 + 186, yPos),
				caDraw::Color(240, 240, 240));
		}

		textArt->endDrawString();
	}
	else
	{
		int bakCharSize = m_font->getCharacterSize();
		m_font->setCharacterSize(64);

		textArt->beginDrawString(*m_font);

		textArt->drawString("SCORE: " + std::to_string(m_score),
			caDraw::PointF(m_panel->size.width / 2, m_panel->size.height / 2 - 128),
			caDraw::Color(255, 49, 38),
			caDraw::TextAligns::Center);

		textArt->endDrawString();

		m_font->setCharacterSize(bakCharSize);
	}
}


void RankScene::onDrawFront(caDraw::Graphics& g)
{
	if (m_rankLoading)
	{
		auto& textArt = g.textArtist;


		textArt->beginDrawString(*m_font);

		textArt->drawString("UPLOADING...",
			caDraw::PointF(m_panel->size.width / 2, m_panel->size.height / 2 + 32),
			caDraw::Color(240, 240, 240),
			caDraw::TextAligns::Center);

		textArt->endDrawString();
	}
}

