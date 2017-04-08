#ifndef __SERVAIVAL__EFFECT_H__
#define __SERVAIVAL__EFFECT_H__


#include <CodeAdapter\EasyCA.h>




class Effect : public caDraw::Drawable
{
private:
	USING_CA_DRAWING(Graphics);
	USING_CA_DRAWING(Transform);


public:
	explicit Effect(const caDraw::VectorF& speed);


private:
	caDraw::TexturePtr m_texBody;
	caDraw::VectorF m_speed;


public:
	void load(const caDraw::TexturePtr& texBody);
	void update();
	virtual void onDraw(Graphics& g, const Transform& parentTransform) override;
};


#endif