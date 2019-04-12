#include "PipePair.h"

/// <summary>
/// PipePair constructor function
/// </summary>
/// <param name="xPositionIn">X position of the pipe pair</param>
/// <param name="screenHeight">Height of the screen</param>
/// <param name="pipeCount">Total number of pipes</param>
/// <param name="gapBetween">The gap between each pipe</param>
PipePair::PipePair(float xPositionIn, int screenHeight, int pipeCount, float gapBetween)
	: m_xPosition(xPositionIn),
	m_screenHeight(screenHeight),
	m_totalPipes(pipeCount),
	m_distanceBetween(gapBetween)
{
	m_upperPipe.setFillColor(sf::Color(29, 46, 72));
	m_lowerPipe.setFillColor(sf::Color(29, 46, 72));
	randomise();
}

/// <summary>
/// PipePair class destructor
/// </summary>
PipePair::~PipePair()
{
}

/// <summary>
/// PipePair update function
/// </summary>
void PipePair::update()
{
	movePipe(m_upperPipe);
	movePipe(m_lowerPipe);
	sf::Vector2f upperPos = m_upperPipe.getPosition();
	sf::Vector2f lowerPos = m_lowerPipe.getPosition();
	m_xPosition = upperPos.x;
	if (upperPos.x < -RECT_WIDTH && lowerPos.x < -RECT_WIDTH)
	{
		m_xPosition = upperPos.x + m_totalPipes * m_distanceBetween;
		randomise();
	}
}

/// <summary>
/// PipePair window render function
/// </summary>
/// <param name="window">Window to draw to</param>
void PipePair::render(sf::RenderWindow & window)
{
	window.draw(m_upperPipe);
	window.draw(m_lowerPipe);
}

/// <summary>
/// PipePair texture render function
/// </summary>
/// <param name="renderTexture">Texture to draw to</param>
void PipePair::renderToTexture(sf::RenderTexture & renderTexture)
{
	renderTexture.draw(m_upperPipe);
	renderTexture.draw(m_lowerPipe);
}

/// <summary>
/// Function randomises the height of the gap between the pair of pipes
/// </summary>
void PipePair::randomise()
{
	m_gapYPosition = (rand() % (m_screenHeight - 400));
	m_upperPipe.setPosition(sf::Vector2f(m_xPosition, 0));
	m_upperPipe.setSize(sf::Vector2f(RECT_WIDTH, m_gapYPosition));
	m_lowerPipe.setPosition(sf::Vector2f(m_xPosition, m_gapYPosition + GAP_WIDTH));
	m_lowerPipe.setSize(sf::Vector2f(RECT_WIDTH, m_screenHeight - m_gapYPosition));
}

/// <summary>
/// Function used to check for a collision between a pipe pair 
/// </summary>
/// <param name="shape">shape against which to check for a collision</param>
/// <returns>Boolean represnting whether or not there is a collision</returns>
bool PipePair::checkCollision(sf::FloatRect shape)
{
	if (m_upperPipe.getGlobalBounds().intersects(shape)
		|| m_lowerPipe.getGlobalBounds().intersects(shape))
	{
		return true;
	}
	return false;
}

/// <summary>
/// Function gets the X position of the pipe pair
/// </summary>
/// <returns></returns>
float PipePair::getX()
{
	return m_xPosition;
}

/// <summary>
/// Function gets the Y position of the gap between the pipe pair
/// </summary>
/// <returns>The height of the gap</returns>
float PipePair::getGapY()
{
	return m_gapYPosition;
}

/// <summary>
/// Fuction used to move the pipe pair rect
/// </summary>
/// <param name="rect">Reference to the pipe pair ractangle</param>
void PipePair::movePipe(sf::RectangleShape & rect)
{
	sf::Vector2f pos = rect.getPosition();
	pos.x -= 3;
	m_xPosition = pos.x;
	rect.setPosition(pos);
}
