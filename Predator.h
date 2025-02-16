#pragma once
#include <SFML/Graphics.hpp>
#include <random>
#include "settings.h"
#include "Utils.h"
#include "LifeUnit.h"

class Predator : public LifeUnit
{
public:

  Predator(float x, float y) : LifeUnit(x, y, LIFE_LEVEL_START, PREDATOR_MAX_SPEED) {}

  void draw(sf::RenderWindow& window)
  {
    sf::CircleShape shape(7);
    shape.setPosition(getPosition());
    int shade = static_cast<int>(getLifeLevel() / LIFE_LEVEL_MAX * 255);
    shape.setFillColor(sf::Color(shade, shade, shade));
    window.draw(shape);
  }
};

