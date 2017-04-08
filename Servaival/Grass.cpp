#include "Grass.h"

#include <random>




Grass::Grass()
	: m_texIndex(std::random_device()() % m_texGrass.size())
	, m_aniGage(0)
{

}

//########################################################################################################

void Grass::load(caUtil::ResourcePool& pool)
{
	for (std::size_t i = 0; i < m_texGrass.size(); ++i)
	{
		std::string name = "grass" + std::to_string(i);

		m_texGrass[i] = pool.getTexture(name);
	}
}


void Grass::update()
{
	if (m_aniGage <= 0)
	{
		m_aniGage = 32;


		++m_texIndex;
		if (m_texIndex >= m_texGrass.size())
		{
			m_texIndex = 0;
		}
	}
	else
	{
		--m_aniGage;
	}
}


void Grass::onDraw(Graphics& g, const Transform& parentTransform)
{
	auto& texArt = g.textureArtist;


	texArt->beginDrawTexture();

	texArt->drawTexture(*m_texGrass[m_texIndex], caDraw::PointF(0, 0));

	texArt->endDrawTexture();
}

