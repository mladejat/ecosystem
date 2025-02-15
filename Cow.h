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

  Cow(float x, float y) : LifeUnit(x, y, 1.0) {}
  Cow(float x, float y, float lifeLevel) : LifeUnit(x, y, lifeLevel) {}

  sf::Vector2f searchHighGrass(GrassCell field[GRID_WIDTH][GRID_HEIGHT]);

  void draw(sf::RenderWindow& window)
  {
    sf::CircleShape shape(5);
    shape.setPosition(getPosition());
    shape.setFillColor(sf::Color(255, 0, 0, static_cast<int>(getLifeLevel() / LIFE_LEVEL_MAX * 255)));
    window.draw(shape);
  }
};