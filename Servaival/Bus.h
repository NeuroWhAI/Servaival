#ifndef __SERVAIVAL__BUS_H__
#define __SERVAIVAL__BUS_H__


#include <CodeAdapter\EasyCA.h>




class Bus : public caDraw::Drawable
{
private:
	USING_CA_DRAWING(Graphics);
	USING_CA_DRAWING(Transform);


public:
	Bus();


private:
	caDraw::TexturePtr m_texBody;
	caDraw::TexturePtr m_texWheel;


private:
	float m_vibratingValue;
	int m_vibrationGage;


public:
	void load(const caDraw::TexturePtr& texBody, const caDraw::TexturePtr& texWheel);
	void update();
	virtual void onDraw(Graphics& g, const Transform& parentTransform) override;
};


#endif