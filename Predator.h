#pragma once
#include <SFML/Graphics.hpp>
#include <random>
#include "settings.h"
#include "Utils.h"

class Predator
{
public:
  sf::Vector2f position;
  float energy;

  Predator(float x, float y) : position(x, y), energy(1.0f) {}

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
    position.x += x;
    position.y += y;

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

