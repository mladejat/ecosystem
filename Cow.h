#pragma once
#include <SFML/Graphics.hpp>
#include <random>
#include "settings.h"
#include "Utils.h"
#include "LifeUnit.h"
#include "GrassCell.h"

class Cow : public LifeUnit
{
public:
  //sf::Vector2f position;
  //float energy;

  Cow(float x, float y) : LifeUnit(x, y, LIFE_LEVEL_START, COW_MAX_SPEED) {}
  Cow(float x, float y, float lifeLevel, float speed) : LifeUnit(x, y, lifeLevel, speed) {}

  sf::Vector2f searchHighGrass(GrassCell field[GRID_WIDTH][GRID_HEIGHT]);

  void draw(sf::RenderWindow& window)
  {
    sf::CircleShape shape(5);
    shape.setPosition(getPosition());
    shape.setFillColor(sf::Color(255, 0, 0, static_cast<int>(getLifeLevel() / LIFE_LEVEL_MAX * 255)));
    window.draw(shape);
  }
};