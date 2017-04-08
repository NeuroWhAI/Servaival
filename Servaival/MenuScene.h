#ifndef __SERVAIVAL__MENU_SCENE_H__
#define __SERVAIVAL__MENU_SCENE_H__


#include <CodeAdapter\EasyCA.h>




class MenuScene : public caUtil::Scene
{
public:
	explicit MenuScene(const caUtil::ResourcePool& resPool);
	virtual ~MenuScene();


private:
	caUtil::ResourcePool m_pool;
	caDraw::FontPtr m_font;
	caDraw::TexturePtr m_texTitle;
	caDraw::TexturePtr m_texToStart;
	caAudio::SoundPtr m_sndBgm;


private:
	caDraw::PanelPtr m_panel;


public:
	virtual void onInitialize(caDraw::Window& owner) override;
	virtual void onRelease() override;
	virtual void onUpdate(caDraw::Window& owner) override;
	virtual void onDrawBack(caDraw::Graphics& g) override;
	virtual void onDrawFront(caDraw::Graphics& g) override;
};


#endif