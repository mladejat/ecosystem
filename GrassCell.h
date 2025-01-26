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
  GrassCell(float x, float y, float lifeLevel)
    :LifeUnit(x, y, lifeLevel)
  {
  };

  void growUp();
  float eatGrass();
};

