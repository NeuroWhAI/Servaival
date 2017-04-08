#ifndef __SERVAIVAL__RANK_SCENE_H__
#define __SERVAIVAL__RANK_SCENE_H__


#include <future>
#include <thread>
#include <mutex>
#include <vector>

#include <CodeAdapter\EasyCA.h>




class RankScene : public caUtil::Scene
{
private:
	class RankingData
	{
	public:
		std::string name;
		std::size_t score;
	};


public:
	RankScene(const caUtil::ResourcePool& resPool, std::size_t score);
	virtual ~RankScene();


private:
	caUtil::ResourcePool m_pool;
	caDraw::FontPtr m_font;
	caDraw::DrawableTexturePtr m_texGameover;


private:
	caDraw::PanelPtr m_panel;
	caUI::TextBoxPtr m_boxName;


private:
	const std::size_t m_score;


private:
	std::future<void> m_taskLoadRanking;
	bool m_rankLoading;
	std::vector<RankingData> m_rankList;
	bool m_rankLoaded;


public:
	virtual void onInitialize(caDraw::Window& owner) override;
	virtual void onRelease() override;
	virtual void onUpdate(caDraw::Window& owner) override;
	virtual void onDrawBack(caDraw::Graphics& g) override;
	virtual void onDrawFront(caDraw::Graphics& g) override;
};


#endif