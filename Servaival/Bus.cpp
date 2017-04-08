#include "Bus.h"




Bus::Bus()
	: m_vibratingValue(0.5f)
	, m_vibrationGage(0)
{

}

//########################################################################################################

void Bus::load(const caDraw::TexturePtr& texBody, const caDraw::TexturePtr& texWheel)
{
	m_texBody = texBody;
	m_texWheel = texWheel;
}


void Bus::update()
{
	if (m_vibrationGage <= 0)
	{
		m_vibrationGage = 5;


		if (m_vibratingValue > 0.0f)
		{
			m_vibratingValue = -0.5f;
		}
		else
		{
			m_vibratingValue = 0.5f;
		}
	}
	else
	{
		--m_vibrationGage;
	}
}


void Bus::onDraw(Graphics& g, const Transform& parentTransform)
{
	auto& texArt = g.textureArtist;


	texArt->beginDrawTexture();

	texArt->drawTexture(*m_texWheel, 0.0f, 0.0f);
	texArt->drawTexture(*m_texBody, caDraw::PointF(0, m_vibratingValue));

	texArt->endDrawTexture();
}

