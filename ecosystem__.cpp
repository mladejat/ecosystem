#include <SFML/Graphics.hpp>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <cmath>

const int fieldWidth = 20;  // Number of columns
const int fieldHeight = 15; // Number of rows
const int cellSize = 40;    // Size of each cell (pixel)

const float maxHealth = 100.0f;
const float maxEnergy = 50.0f;
const float energyPerGrass = 10.0f;
const float predatorEnergyGain = 30.0f;
const float cowEnergyLossWithoutGrass = 0.1f;
const float predatorEnergyLossWithoutCows = 0.1f;

class GrassCell {
public:
  GrassCell() : growthStage(5) {}  // Start with maximum growth

  void grow() {
    if (growthStage < 5) {
      growthStage++;
    }
  }

  void eatGrass() {
    if (growthStage > 0) {
      growthStage--;
    }
  }

  sf::Color getColor() const {
    switch (growthStage) {
    case 0: return sf::Color(144, 238, 144);  // light green
    case 1: return sf::Color(124, 204, 124);
    case 2: return sf::Color(104, 170, 104);
    case 3: return sf::Color(84, 136, 84);
    case 4: return sf::Color(64, 102, 64);
    case 5: return sf::Color(44, 68, 44);  // darkest green
    default: return sf::Color(144, 238, 144);
    }
  }

  bool hasGrass() const {
    return growthStage > 0;
  }

  int getGrassLevel() const {
    return growthStage; // Return the grass level (0 to 5)
  }

private:
  int growthStage;  // Represents the growth stage (0 to 5)
};

class Cow {
public:
  Cow(int x, int y) : posX(x), posY(y), health(maxHealth), energy(maxEnergy) {}

  void move() {
    // Random movement (simplified AI)
    int direction = std::rand() % 4;
    switch (direction) {
    case 0: if (posX > 0) posX--; break;  // Move left
    case 1: if (posX < fieldWidth - 1) posX++; break;  // Move right
    case 2: if (posY > 0) posY--; break;  // Move up
    case 3: if (posY < fieldHeight - 1) posY++; break;  // Move down
    }
  }

  void eatGrass(GrassCell& grass) {
    if (grass.hasGrass()) {
      grass.eatGrass();
      energy += energyPerGrass;
      if (energy > maxEnergy) energy = maxEnergy;
    }
  }

  void loseEnergy(float amount) {
    energy -= amount;
    if (energy < 0) energy = 0;
  }

  bool isAlive() const {
    return health > 0 && energy > 0;
  }

  void update() {
    if (energy <= 0) health -= 10;  // If no energy, lose health over time
  }

  void draw(sf::RenderWindow& window) {
    sf::CircleShape shape(12);
    shape.setFillColor(sf::Color(255, 255, 255));  // White for cows
    shape.setPosition(posX * cellSize + 10, posY * cellSize + 10);  // Offset for better visibility
    window.draw(shape);
  }

  int posX, posY;
  float health, energy;
};

class Predator {
public:
  Predator(int x, int y) : posX(x), posY(y), health(maxHealth), energy(maxEnergy) {}

  void move() {
    // Random movement (simplified AI)
    int direction = std::rand() % 4;
    switch (direction) {
    case 0: if (posX > 0) posX--; break;  // Move left
    case 1: if (posX < fieldWidth - 1) posX++; break;  // Move right
    case 2: if (posY > 0) posY--; break;  // Move up
    case 3: if (posY < fieldHeight - 1) posY++; break;  // Move down
    }
  }

  void catchCow(Cow& cow) {
    if (cow.posX == posX && cow.posY == posY) {
      energy += predatorEnergyGain;
      if (energy > maxEnergy) energy = maxEnergy;
      cow.health = 0;  // Predator kills the cow
    }
  }

  void loseEnergy(float amount) {
    energy -= amount;
    if (energy < 0) energy = 0;
  }

  bool isAlive() const {
    return health > 0 && energy > 0;
  }

  void update() {
    if (energy <= 0) health -= 10;  // If no energy, lose health over time
  }

  bool detectCow(const Cow& cow, const std::vector<std::vector<GrassCell>>& field) {
    // Check the surrounding area for grass and cows (based on grass level)
    int detectionRadius = 2;  // Distance within which predator can sense cows
    for (int dy = -detectionRadius; dy <= detectionRadius; ++dy) {
      for (int dx = -detectionRadius; dx <= detectionRadius; ++dx) {
        int nx = cow.posX + dx;
        int ny = cow.posY + dy;
        if (nx >= 0 && nx < fieldWidth && ny >= 0 && ny < fieldHeight) {
          if (abs(dx) + abs(dy) <= detectionRadius && field[ny][nx].getGrassLevel() > 2) {
            // Predators are more likely to detect cows in areas with higher grass levels
            return true;
          }
        }
      }
    }
    return false;
  }

  void draw(sf::RenderWindow& window) {
    sf::CircleShape shape(10);
    shape.setFillColor(sf::Color(255, 0, 0));  // Red for predators
    shape.setPosition(posX * cellSize + 10, posY * cellSize + 10);  // Offset for better visibility
    window.draw(shape);
  }

  int posX, posY;
  float health, energy;
};

int main() {
  sf::RenderWindow window(sf::VideoMode(fieldWidth * cellSize, fieldHeight * cellSize), "Grass, Cows, and Predators");

  // Initialize the field of grass
  std::vector<std::vector<GrassCell>> field(fieldHeight, std::vector<GrassCell>(fieldWidth));

  // Initialize cows and predators
  std::vector<Cow> cows = { Cow(5, 5), Cow(10, 10), Cow(15, 5) };
  std::vector<Predator> predators = { Predator(7, 7), Predator(12, 12) };

  // Clock for controlling game logic
  sf::Clock clock;
  std::srand(static_cast<unsigned>(std::time(0)));

  while (window.isOpen()) {
    sf::Event event;
    while (window.pollEvent(event)) {
      if (event.type == sf::Event::Closed)
        window.close();
    }

    // Game logic (move cows and predators, handle energy and health)
    if (clock.getElapsedTime().asSeconds() > 1.0f) {
      for (auto& cow : cows) {
        cow.move();
        cow.update();
        if (field[cow.posY][cow.posX].hasGrass()) {
          cow.eatGrass(field[cow.posY][cow.posX]);
        }
        if (!cow.isAlive()) {
          // Remove dead cows (simplified here, can be extended)
          cow = cows.back();
          cows.pop_back();
        }
      }

      for (auto& predator : predators) {
        predator.move();
        predator.update();
        if (!predator.isAlive()) {
          // Remove dead predators
          predator = predators.back();
          predators.pop_back();
        }

        // Check if predators catch any cows based on grass level
        for (auto& cow : cows) {
          if (predator.detectCow(cow, field)) {
            predator.catchCow(cow);
          }
        }

        if (predator.energy <= 0) predator.loseEnergy(predatorEnergyLossWithoutCows);
      }

      clock.restart();
    }

    // Clear the window
    window.clear(sf::Color::White);

    // Draw grass
    for (int y = 0; y < fieldHeight; ++y) {
      for (int x = 0; x < fieldWidth; ++x) {
        sf::RectangleShape cell(sf::Vector2f(cellSize, cellSize));
        cell.setPosition(x * cellSize, y * cellSize);
        cell.setFillColor(field[y][x].getColor());
        window.draw(cell);
      }
    }

    // Draw cows
    for (auto& cow : cows) {
      cow.draw(window);
    }

    // Draw predators
    for (auto& predator : predators) {
      predator.draw(window);
    }

    // Display everything
    window.display();
  }

  return 0;
}
