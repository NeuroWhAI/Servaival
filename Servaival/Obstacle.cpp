#include "Obstacle.h"




Obstacle::Obstacle()
	: m_dead(false)
{

}

//########################################################################################################

void Obstacle::load(const caDraw::TexturePtr& texBody)
{
	m_texBody = texBody;
}


void Obstacle::update()
{
	
}


void Obstacle::onDraw(Graphics& g, const Transform& parentTransform)
{
	auto& texArt = g.textureArtist;


	texArt->beginDrawTexture();

	texArt->drawTexture(*m_texBody, caDraw::PointF(0, 0));

	texArt->endDrawTexture();
}

//########################################################################################################

bool Obstacle::isDead() const
{
	return m_dead;
}


void Obstacle::die()
{
	m_dead = true;
}

