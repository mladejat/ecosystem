#pragma once
#include <SFML/Graphics.hpp>
#include <random>
#include "settings..h"
#include "Utils.h"

class Predator
{
public:
  sf::Vector2f position;
  float energy;

  Predator(float x, float y) : position(x, y), energy(1.0f) {}

  void move()
  {
    position.x += Utils::getRandomFloat(-1.5f, 1.5f);
    position.y += Utils::getRandomFloat(-1.5f, 1.5f);
    energy -= PREDATOR_ENERGY_DECAY;
  }

  void draw(sf::RenderWindow& window)
  {
    sf::CircleShape shape(7);
    shape.setPosition(position);
    int shade = static_cast<int>(energy * 255);
    shape.setFillColor(sf::Color(shade, shade, shade));
    window.draw(shape);
  }
};

