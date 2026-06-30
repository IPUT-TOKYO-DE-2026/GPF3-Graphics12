#include "GameObject.h"

GameObject::GameObject()
	: centerX(0), centerY(0), speedX(0), speedY(0), isActive(true), isAlreadyHit(false)
{

}

GameObject::GameObject(float x, float y, float spx, float spy)
	: centerX(x), centerY(y), speedX(spx), speedY(spy), isActive(true), isAlreadyHit(false)
{
}

void GameObject::setColor(int b, int g, int r)
{
	color[0] = b;
	color[1] = g;
	color[2] = r;
}

void GameObject::setSpeed(float x, float y)
{
	speedX = x;
	speedY = y;
}


void GameObject::setCenter(float x, float y)
{
	centerX = x;
	centerY = y;

	calcBoundingBox();
}

void GameObject::move()
{
	centerX += speedX;
	centerY += speedY;

	calcBoundingBox();
}
