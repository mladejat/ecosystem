#include "Cow.h"

sf::Vector2f Cow::searchHighGrass(GrassCell field[GRID_WIDTH][GRID_HEIGHT])
{
  auto startX = static_cast<int>(std::max(0.0f, (this->getX() - COW_VICINITY_RADIUS ) / CELL_SIZE));
  auto endX = static_cast<int>(std::min(static_cast<float>(WINDOW_WIDTH), (this->getX() + COW_VICINITY_RADIUS ) / CELL_SIZE));
  auto startY = static_cast<int>(std::max(0.0f, (this->getY() - COW_VICINITY_RADIUS ) / CELL_SIZE));
  auto endY = static_cast<int>(std::min(static_cast<float>(WINDOW_HEIGHT), (this->getY() + COW_VICINITY_RADIUS ) / CELL_SIZE));

  auto grassLevel = 0.0f;
  int ii = 0, jj = 0;

  for (int i = startX; i < endX; ++i)
    for (int j = startY; j < endY; ++j)
    {
      auto gl = field[i][j].getLifeLevel();
      if (gl >= grassLevel)
      {
        grassLevel = gl;
        ii = i;
        jj = j;
      }
    }
  sf::Vector2f position;
  position.x = static_cast<float>(ii * CELL_SIZE + CELL_SIZE / 2);
  position.y = static_cast<float>(jj* CELL_SIZE + CELL_SIZE / 2);

  return position;
}
