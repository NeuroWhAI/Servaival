#ifndef __SERVAIVAL__GRASS_H__
#define __SERVAIVAL__GRASS_H__


#include <array>

#include <CodeAdapter\EasyCA.h>




class Grass : public caDraw::Drawable
{
private:
	USING_CA_DRAWING(Graphics);
	USING_CA_DRAWING(Transform);


public:
	Grass();


private:
	std::array<caDraw::TexturePtr, 3> m_texGrass;


private:
	std::size_t m_texIndex;
	int m_aniGage;


public:
	void load(caUtil::ResourcePool& pool);
	void update();
	virtual void onDraw(Graphics& g, const Transform& parentTransform) override;
};


#endif