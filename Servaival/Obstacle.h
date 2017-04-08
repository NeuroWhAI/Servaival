#ifndef __SERVAIVAL__OBSTACLE_H__
#define __SERVAIVAL__OBSTACLE_H__


#include <CodeAdapter\EasyCA.h>




class Obstacle : public caDraw::Drawable
{
private:
	USING_CA_DRAWING(Graphics);
	USING_CA_DRAWING(Transform);


public:
	Obstacle();


private:
	caDraw::TexturePtr m_texBody;
	bool m_dead;


public:
	void load(const caDraw::TexturePtr& texBody);
	void update();
	virtual void onDraw(Graphics& g, const Transform& parentTransform) override;


public:
	bool isDead() const;
	void die();
};


#endif