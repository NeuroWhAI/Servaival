#ifndef __SERVAIVAL__SERVAL_H__
#define __SERVAIVAL__SERVAL_H__


#include <CodeAdapter\EasyCA.h>




class Serval : public caDraw::Drawable
{
private:
	USING_CA_DRAWING(Graphics);
	USING_CA_DRAWING(Transform);


public:
	Serval();


private:
	caDraw::TexturePtr m_texBody;


private:
	float m_vibratingValue;
	int m_vibrationGage;


private:
	float m_beforeJump;
	bool m_canJump;
	caDraw::VectorF m_speed;


public:
	void load(const caDraw::TexturePtr& texBody);
	void update();
	virtual void onDraw(Graphics& g, const Transform& parentTransform) override;


public:
	bool canJump() const;
	void jump();
	void restoreYToBeforeJump();
	void hit();
};


#endif