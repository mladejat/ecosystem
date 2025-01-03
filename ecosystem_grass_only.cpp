#include <SFML/Graphics.hpp>
#include <vector>

const int fieldWidth = 20;  // Number of columns
const int fieldHeight = 15; // Number of rows
const int cellSize = 40;    // Size of each cell (pixel)

class GrassCell {
public:
  GrassCell() : growthStage(0) {}

  void grow() {
    if (growthStage < 5) {
      growthStage++;
    }
  }

  sf::Color getColor() const {
    // Gradually darkens the green as the grass grows
    switch (growthStage) {
    case 0: return sf::Color(144, 238, 144); // light green
    case 1: return sf::Color(124, 204, 124);
    case 2: return sf::Color(104, 170, 104);
    case 3: return sf::Color(84, 136, 84);
    case 4: return sf::Color(64, 102, 64);
    case 5: return sf::Color(44, 68, 44);  // darkest green
    default: return sf::Color(144, 238, 144);
    }
  }

  void update() {
    grow();
  }

private:
  int growthStage;  // Represents the growth stage (0 to 5)
};

int main() {
  // Create a window
  sf::RenderWindow window(sf::VideoMode(fieldWidth * cellSize, fieldHeight * cellSize), "Growing Grass");

  // Create a grid of GrassCells
  std::vector<std::vector<GrassCell>> field(fieldHeight, std::vector<GrassCell>(fieldWidth));

  // Clock for controlling growth speed
  sf::Clock clock;

  while (window.isOpen()) {
    sf::Event event;
    while (window.pollEvent(event)) {
      if (event.type == sf::Event::Closed)
        window.close();
    }

    // Update all cells' growth
    if (clock.getElapsedTime().asSeconds() > 1.0f) {  // Grow every 1 second
      for (int y = 0; y < fieldHeight; ++y) {
        for (int x = 0; x < fieldWidth; ++x) {
          field[y][x].update();
        }
      }
      clock.restart();
    }

    // Clear the window
    window.clear(sf::Color::White);

    // Draw the field
    for (int y = 0; y < fieldHeight; ++y) {
      for (int x = 0; x < fieldWidth; ++x) {
        // Create a rectangle to represent each cell of grass
        sf::RectangleShape cell(sf::Vector2f(cellSize, cellSize));
        cell.setPosition(x * cellSize, y * cellSize);
        cell.setFillColor(field[y][x].getColor());

        window.draw(cell);
      }
    }

    // Display everything
    window.display();
  }

  return 0;
}
