#pragma once
#include <SFML/Graphics.hpp>
#include <random>
#include "settings.h"
#include "Utils.h"

class Cow
{
public:
  sf::Vector2f position;
  float energy;

  Cow(float x, float y) : position(x, y), energy(1.0f) {}

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

    energy -= COW_ENERGY_DECAY;
  }

  void draw(sf::RenderWindow& window)
  {
    sf::CircleShape shape(5);
    shape.setPosition(position);
    shape.setFillColor(sf::Color(255, 0, 0, static_cast<int>(energy * 255)));
    window.draw(shape);
  }
};