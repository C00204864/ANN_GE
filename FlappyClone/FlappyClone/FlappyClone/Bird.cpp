#include "Bird.h"

/// <summary>
/// Bird class constructor function
/// </summary>
/// <param name="xIn">X position for the bird</param>
/// <param name="yIn">Y position for the bird</param>
Bird::Bird(float xIn, float yIn)
{
	m_shape.setPosition(sf::Vector2f(xIn, yIn));
	m_shape.setSize(sf::Vector2f(WIDTH, HEIGHT));
	m_shape.setFillColor(sf::Color(13, 191, 10));
}

/// <summary>
/// Bird destructor function
/// </summary>
Bird::~Bird()
{
}

/// <summary>
/// Bird update function
/// </summary>
void Bird::update()
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
	{
		m_yVelocity = -11.f;
	}
	m_yVelocity += 1.f;
	sf::Vector2f pos = m_shape.getPosition();
	pos.y += m_yVelocity;
	m_shape.setPosition(pos);
}

/// <summary>
/// Bird u[date function using ANN
/// </summary>
/// <param name="flap"></param>
void Bird::updateNN(bool flap)
{
	if (flap)
	{
		m_yVelocity = -11.f;
	}
	m_yVelocity += 1.f;
	sf::Vector2f pos = m_shape.getPosition();
	pos.y += m_yVelocity;
	m_shape.setPosition(pos);
}

/// <summary>
/// Bird render function
/// </summary>
/// <param name="window">Redner window to which to draw</param>
void Bird::render(sf::RenderWindow & window)
{
	window.draw(m_shape);
}

/// <summary>
/// Bird render function using render textures
/// </summary>
/// <param name="renderTexture">Render texture to which to draw</param>
void Bird::renderToTexture(sf::RenderTexture & renderTexture)
{
	renderTexture.draw(m_shape);
}

/// <summary>
/// Get the sf::shape representing the bird
/// </summary>
/// <returns>sf\::shape representing the bird</returns>
sf::RectangleShape Bird::getShape()
{
	return m_shape;
}

/// <summary>
/// Set position of the bird
/// </summary>
/// <param name="pos">Desired bird position</param>
void Bird::setPosition(sf::Vector2f pos)
{
	m_shape.setPosition(pos);
	m_yVelocity = 0.f;
}

/// <summary>
/// Increment the score of the bird
/// </summary>
void Bird::incrementScore()
{
	++m_score;
}

/// <summary>
/// Reset the score fo the bird
/// </summary>
void Bird::resetScore()
{
	m_score = 0;
}

/// <summary>
/// Check whether or not the bird is alive
/// </summary>
/// <returns>Boolean representing whether the bird is alive</returns>
bool Bird::isAlive()
{
	return m_alive;
}

/// <summary>
/// Set the alive status of the bird
/// </summary>
/// <param name="status">Desired bird alive status</param>
void Bird::setAliveStatus(bool status)
{
	m_alive = status;
}

/// <summary>
/// Get the y position of the bird
/// </summary>
/// <returns>The y position of the bird</returns>
float Bird::getY()
{
	return m_shape.getPosition().y;
}
