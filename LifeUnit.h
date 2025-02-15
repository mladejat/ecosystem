#pragma once
#include <SFML/Graphics.hpp>
#include <stdint.h>
#include <utility>
#include <vector>
#include <iostream>

class LifeUnit
{
private:
  sf::Vector2f _position;
  float _lifeLevel = 0.0;
public:
  LifeUnit(float x, float y, float lifeLevel)
    : _position{x, y}
    , _lifeLevel{ lifeLevel }
  {

  }
  float getLifeLevel() { return _lifeLevel; }
  float getX() const { return _position.x; }
  float getY() const { return _position.y; }
  sf::Vector2f getPosition() const { return _position; }

  void setLifeLevel(float val) { _lifeLevel = val; }
  void setX(float x) { _position.x = x; }
  void setY(float y) { _position.y = y; }
  void setPosition(sf::Vector2f position) { _position = position; }
  void move(sf::Vector2f refPoint, bool direction);
#define moveToward(x) move(x, true)
#define moveAway(x) move(x, false)

  template <typename T> 
  std::pair<sf::Vector2f, float> findClosest(const std::vector<T>& lifeUnit)
  {
    auto size = lifeUnit.size();
    std::cout << "lifeUnit.size():" << size << ", addr:" << &lifeUnit << "\n";
    if (size && (size < 1000000))
    {
      auto closestPoint = lifeUnit[0].getPosition();
      float minDistanceSquared = std::numeric_limits<float>::max();
      {
        //for (const auto& unit : lifeUnit)
        for (int i = 0; i < lifeUnit.size(); ++i)
        {
          // Calculate squared distance between point (point.first, point.second) and (x, y)
          float dx = std::abs(lifeUnit[i].getX() - this->getX());
          float dy = std::abs(lifeUnit[i].getY() - this->getY());
          float distanceSquared = dx * dx + dy * dy;

          // If this distance is smaller than the previous minimum, update closest point
          if (distanceSquared < minDistanceSquared)
          {
            minDistanceSquared = distanceSquared;
            closestPoint = lifeUnit[i].getPosition();
          }
          //// Calculate squared distance between point (point.first, point.second) and (x, y)
          //float dx = std::abs(unit.getX() - this->getX());
          //float dy = std::abs(unit.getY() - this->getY());
          //float distanceSquared = dx * dx + dy * dy;

          //// If this distance is smaller than the previous minimum, update closest point
          //if (distanceSquared < minDistanceSquared)
          //{
          //  minDistanceSquared = distanceSquared;
          //  closestPoint = unit.getPosition();
          //}

        }
      }
      return { closestPoint, std::sqrt(minDistanceSquared) };
    }
    else
    {
      return { {0, 0}, std::numeric_limits<float>::max() };
    }

  }
};
