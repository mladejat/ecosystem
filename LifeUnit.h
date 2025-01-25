#pragma once
#include <stdint.h>
#include <utility>
#include <vector>

class LifeUnit
{
private:
  uint32_t _row;
  uint32_t _column;
  double _lifeLevel = 0.0;
public:
  LifeUnit(uint32_t row, uint32_t column, double lifeLevel)
    : _row{ row }
    , _column{ column }
    , _lifeLevel{ lifeLevel }
  {

  }
  double getLifeLevel() { return _lifeLevel; }
  uint32_t getRow() const { return _row; }
  uint32_t getColumn() const { return _column; }
  std::pair<uint32_t, uint32_t> getPoint() const { return std::pair<uint32_t, uint32_t>(_row, _column); }
  void setLifeLevel(double val) { _lifeLevel = val; }
  void setRow(uint32_t row) { _row = row; }
  void setColumn(uint32_t column) { _column = column; }

  template <typename T> std::tuple<uint32_t, uint32_t, uint32_t> findClosest(const std::vector<T>& lifeUnit)
  {
    std::pair<uint32_t, uint32_t> closestPoint = lifeUnit[0].getPoint();
    uint32_t minDistanceSquared = std::numeric_limits<uint32_t>::max();

    for (const auto& unit : lifeUnit) {
      // Calculate squared distance between point (point.first, point.second) and (x, y)
      uint32_t dx = std::abs(static_cast<int>(unit.getRow()) - static_cast<int>(this->getRow()));
      uint32_t dy = std::abs(static_cast<int>(unit.getColumn()) - static_cast<int>(this->getColumn()));
      uint32_t distanceSquared = dx * dx + dy * dy;

      // If this distance is smaller than the previous minimum, update closest point
      if (distanceSquared < minDistanceSquared) {
        minDistanceSquared = distanceSquared;
        closestPoint = unit.getPoint();
      }
    }

    return { closestPoint.first, closestPoint.second, minDistanceSquared };
  }
};
