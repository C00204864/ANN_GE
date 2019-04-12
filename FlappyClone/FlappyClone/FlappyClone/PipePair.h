#ifndef PIPEPAIR_H
#define PIPEPAIR_H

#include <SFML/Graphics.hpp>
#include "Bird.h"

/// <summary>
/// Class represents a piar of pipes for the Flappy Bird game
/// </summary>
class PipePair
{
public:
	// Public Functions
	PipePair(float xPositionIn, int screenHeight, int pipeCount, float gapBetween);
	~PipePair();
	void update();
	void render(sf::RenderWindow & window);
	void renderToTexture(sf::RenderTexture & renderTexture);
	void randomise();
	bool checkCollision(sf::FloatRect shape);
	float getX();
	float getGapY();
private:
	// Private Functions
	void movePipe(sf::RectangleShape & rect);

	// Private Members
	sf::RectangleShape m_upperPipe;
	sf::RectangleShape m_lowerPipe;
	float m_xPosition;
	int m_screenHeight;
	int m_totalPipes;
	float m_distanceBetween;
	float m_gapYPosition;
	const float RECT_WIDTH = 45.f;
	const float GAP_WIDTH = 200.f;
};

#endif // !PIPEPAIR_H


