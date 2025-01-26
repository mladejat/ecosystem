#include "settings.h"
#include "GrassCell.h"

void GrassCell::growUp()
{
  if (getLifeLevel() < minLifeLevel)
  {
    setLifeLevel(minLifeLevel);
  }
  else
  {
    setLifeLevel(2 * getLifeLevel());
    if (getLifeLevel() > maxLifeLevel)
    {
      setLifeLevel(maxLifeLevel);
    }
  }
}

float GrassCell::eatGrass()
{
  float ret = getLifeLevel() / 3;
  setLifeLevel(2 * ret);
  return ret;
}
