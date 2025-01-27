#pragma once
#include <SFML/Graphics.hpp>
#include <random>
#include "settings.h"
#include "Utils.h"
#include "LifeUnit.h"

class Cow : public LifeUnit
{
public:
  //sf::Vector2f position;
  //float energy;

  Cow(float x, float y) : LifeUnit(x, y, 1.0) {}
  Cow(float x, float y, float lifeLevel) : LifeUnit(x, y, lifeLevel) {}

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

    setLifeLevel(getLifeLevel() - COW_ENERGY_DECAY);
  }

  void draw(sf::RenderWindow& window)
  {
    sf::CircleShape shape(5);
    shape.setPosition(getPosition());
    shape.setFillColor(sf::Color(255, 0, 0, static_cast<int>(getLifeLevel() / maxLifeLevel * 255)));
    window.draw(shape);
  }
};