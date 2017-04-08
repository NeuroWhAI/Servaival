#include "Effect.h"




Effect::Effect(const caDraw::VectorF& speed)
	: m_speed(speed)
{

}

//########################################################################################################

void Effect::load(const caDraw::TexturePtr& texBody)
{
	m_texBody = texBody;
}


void Effect::update()
{
	transform.position.move(m_speed.x, m_speed.y);

	m_speed.y += 0.8f;
}


void Effect::onDraw(Graphics& g, const Transform& parentTransform)
{
	auto& texArt = g.textureArtist;


	texArt->beginDrawTexture();

	texArt->drawTexture(*m_texBody, caDraw::PointF(0, 0));

	texArt->endDrawTexture();
}

