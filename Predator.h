#pragma once
#include <SFML/Graphics.hpp>
#include <random>
#include "settings.h"
#include "Utils.h"
#include "LifeUnit.h"

class Predator : public LifeUnit
{
public:

  Predator(float x, float y) : LifeUnit(x, y, maxLifeLevel) {}

  void move()
  {
    static int counter = 0;
    static float x = 0.0f;
    static float y = 0.0f;

    if (++counter == 3)
    {
      counter = 0;
      x = Utils::getRandomFloat(-1.0f, 1.0f);
      y = Utils::getRandomFloat(-1.0f, 1.0f);
    }
    setX(getX() + x);
    setY(getY() + y);

    setLifeLevel(getLifeLevel() - PREDATOR_ENERGY_DECAY);
  }

  void draw(sf::RenderWindow& window)
  {
    sf::CircleShape shape(7);
    shape.setPosition(getPosition());
    int shade = static_cast<int>(getLifeLevel() / maxLifeLevel * 255);
    shape.setFillColor(sf::Color(shade, shade, shade));
    window.draw(shape);
  }
};

