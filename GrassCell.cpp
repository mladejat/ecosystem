#include "settings.h"
#include "GrassCell.h"

void GrassCell::growUp()
{
  if (getLifeLevel() < LIFE_LEVEL_MIN)
  {
    setLifeLevel(LIFE_LEVEL_MIN);
  }
  else
  {
    setLifeLevel(1.1f * getLifeLevel());
    if (getLifeLevel() > LIFE_LEVEL_MAX)
    {
      setLifeLevel(LIFE_LEVEL_MAX);
    }
  }
}

float GrassCell::eatGrass()
{
  float ret = getLifeLevel() / 3;
  setLifeLevel(2 * ret);
  return ret;
}
