#ifndef __SERVAIVAL__LOAD_SCENE_H__
#define __SERVAIVAL__LOAD_SCENE_H__


#include <string>
#include <fstream>

#include <CodeAdapter\EasyCA.h>




class LoadScene : public caUtil::Scene
{
public:
	LoadScene();
	virtual ~LoadScene();


private:
	caUtil::ResourcePool m_pool;
	caDraw::FontPtr m_font;


private:
	caDraw::PanelPtr m_panel;


private:
	std::ifstream m_loadInfo;
	std::string m_loadFileName;
	int m_loadStage;
	int m_loadGage;


public:
	virtual void onInitialize(caDraw::Window& owner) override;
	virtual void onRelease() override;
	virtual void onUpdate(caDraw::Window& owner) override;
	virtual void onDrawBack(caDraw::Graphics& g) override;
	virtual void onDrawFront(caDraw::Graphics& g) override;
};


#endif