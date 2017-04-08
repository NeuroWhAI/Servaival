#include "Serval.h"




Serval::Serval()
	: m_vibratingValue(0.5f)
	, m_vibrationGage(0)

	, m_beforeJump(0)
	, m_canJump(true)
	, m_speed(0, 0)
{

}

//########################################################################################################

void Serval::load(const caDraw::TexturePtr& texBody)
{
	m_texBody = texBody;
}


void Serval::update()
{
	transform.position.move(m_speed.x, m_speed.y);

	m_speed.x *= 0.9f;
	if (std::abs(m_speed.x) < 0.1f)
	{
		m_speed.x = 0.0f;
	}


	if (m_canJump == false)
	{
		m_speed.y += 2.0f;

		if (transform.position.y >= m_beforeJump)
		{
			transform.position.y = m_beforeJump;

			m_speed.y = 0.0f;

			m_canJump = true;
		}
	}
	else
	{
		m_beforeJump = transform.position.y;


		if (caKeyboard->isKeyDown(caSys::Keys::Right) || caKeyboard->isKeyPressed(caSys::Keys::Right))
		{
			transform.scale = { -1, 1 };


			if (m_vibrationGage <= 0)
			{
				m_vibrationGage = 7;


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
	}
}


void Serval::onDraw(Graphics& g, const Transform& parentTransform)
{
	auto& texArt = g.textureArtist;


	texArt->beginDrawTexture();

	texArt->drawTexture(*m_texBody, caDraw::PointF(0, m_vibratingValue));

	texArt->endDrawTexture();
}

//########################################################################################################

bool Serval::canJump() const
{
	return m_canJump;
}


void Serval::jump()
{
	if (m_canJump)
	{
		m_canJump = false;

		m_beforeJump = transform.position.y;

		m_speed.y = -16.0f;
	}
}


void Serval::restoreYToBeforeJump()
{
	transform.position.y = m_beforeJump;
}


void Serval::hit()
{
	m_speed.x = -8.0f;
}

