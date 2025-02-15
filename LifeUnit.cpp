#include "LifeUnit.h"
#include "settings.h"

void LifeUnit::move(sf::Vector2f refPoint, bool direction)
{

  auto x = this->getX();
  auto y = this->getY();

  auto refX = refPoint.x;
  auto refY = refPoint.y;

  if ((x < refX) && direction) // toward
  {
    x += _lifeLevel;
  }
  else if ((x > refX) && direction) // toward
  {
    x -= _lifeLevel;
  }
  else if ((x < refX) && !direction) // away
  {
    x -= _lifeLevel;
  }
  else if ((x > refX) && !direction) // away
  {
    x += _lifeLevel;
  }

  if ((y < refY) && direction) // toward
  {
    y += _lifeLevel;
  }
  else if ((y > refY) && direction) // toward
  {
    y -= _lifeLevel;
  }
  else if ((y < refY) && !direction) // away
  {
    y -= _lifeLevel;
  }
  else if ((y > refY) && !direction) // away
  {
    y += _lifeLevel;
  }

  if (x < 0.0f) x = 0.0f;
  if (y < 0.0f) y = 0.0f;
  if (x > WINDOW_WIDTH) x = WINDOW_WIDTH;
  if (y > WINDOW_HEIGHT) y = WINDOW_HEIGHT;

  this->setX(x);
  this->setY(y);
  this->setLifeLevel(this->getLifeLevel() - MOVE_ENERGY_DECAY);
}
