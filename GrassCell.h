#pragma once

#include <stdint.h>
#include "LifeUnit.h"

class GrassCell : public LifeUnit
{
public:
  GrassCell()
    :LifeUnit(0, 0, 0)
  {
  };
  GrassCell(uint32_t row, uint32_t column, double lifeLevel)
    :LifeUnit(row, column, lifeLevel)
  {
  };

  void growUp();
  double eatGrass();
};

