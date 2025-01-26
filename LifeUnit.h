#pragma once
#include <SFML/Graphics.hpp>
#include <stdint.h>
#include <utility>
#include <vector>

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
  sf::Vector2f getPosition() { return _position; }

  void setLifeLevel(float val) { _lifeLevel = val; }
  void setX(float x) { _position.x = x; }
  void setY(float y) { _position.y = y; }
  void setPosition(sf::Vector2f position) { _position = position; }

  template <typename T> std::pair<sf::Vector2f, float> findClosest(const std::vector<T>& lifeUnit)
  {
    sf::Vector2f closestPoint = lifeUnit[0].getPosition();
    float minDistanceSquared = std::numeric_limits<float>::max();

    for (const auto& unit : lifeUnit) {
      // Calculate squared distance between point (point.first, point.second) and (x, y)
      float dx = std::abs(unit.getX() - this->getX());
      float dy = std::abs(unit.getY() - this->getY());
      float distanceSquared = dx * dx + dy * dy;

      // If this distance is smaller than the previous minimum, update closest point
      if (distanceSquared < minDistanceSquared) {
        minDistanceSquared = distanceSquared;
        closestPoint = unit.getPosition();
      }
    }

    return { closestPoint.first, closestPoint.second, minDistanceSquared };
  }
};
