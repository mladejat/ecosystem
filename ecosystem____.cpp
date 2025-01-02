#include <SFML/Graphics.hpp>
#include <vector>
#include <thread>
#include <chrono>
#include <random>
#include <iostream>

const int FIELD_WIDTH = 800;
const int FIELD_HEIGHT = 600;
const int NUM_COWS = 5;
const int NUM_PREDATORS = 3;

class Grass {
public:
  Grass(int width, int height);

  void grow();
  void render(sf::RenderWindow& window);
  int getGrassAt(int x, int y) const;
  void setGrassAt(int x, int y, int value);

  int getWidth() const { return width; }
  int getHeight() const { return height; }

private:
  int width, height;
  std::vector<std::vector<int>> field;
};

class Cow {
public:
  Cow();

  void moveAndEat(Grass& field);
  void move();
  void eat(Grass& field);
  void render(sf::RenderWindow& window);
  sf::Vector2f getPosition() const;
  void die();

private:
  float energy;
  sf::Vector2f position;
  sf::Color color;
};

class Predator {
public:
  Predator();

  void huntAndMove(Grass& field, std::vector<Cow>& cows);
  void move();
  void hunt(std::vector<Cow>& cows);
  void render(sf::RenderWindow& window);
  sf::Vector2f getPosition() const;

private:
  float energy;
  sf::Vector2f position;
  sf::Color color;
};

// Grass class methods
Grass::Grass(int width, int height) : width(width), height(height) {
  field.resize(height, std::vector<int>(width, 0));  // Initialize the grass field
}

void Grass::grow() {
  while (true) {
    // Randomly grow grass at empty spots
    for (int y = 0; y < height; ++y) {
      for (int x = 0; x < width; ++x) {
        if (field[y][x] < 255) {
          field[y][x] += rand() % 3; // Simulate grass growth
        }
      }
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(500));  // Growth every half second
  }
}

void Grass::render(sf::RenderWindow& window) {
  for (int y = 0; y < height; ++y) {
    for (int x = 0; x < width; ++x) {
      sf::RectangleShape rectangle(sf::Vector2f(1, 1));  // Each pixel is 1x1 rectangle
      rectangle.setPosition(x, y);
      int greenValue = field[y][x];
      rectangle.setFillColor(sf::Color(0, greenValue, 0));  // Green color with saturation based on growth
      window.draw(rectangle);
    }
  }
}

int Grass::getGrassAt(int x, int y) const {
  if (x >= 0 && x < width && y >= 0 && y < height) {
    return field[y][x];
  }
  return 0;  // If out of bounds, return 0 (no grass)
}

void Grass::setGrassAt(int x, int y, int value) {
  if (x >= 0 && x < width && y >= 0 && y < height) {
    field[y][x] = value;
  }
}

// Cow class methods
Cow::Cow() : energy(100), position(rand() % 800, rand() % 600), color(sf::Color::Red) {}

void Cow::moveAndEat(Grass& field) {
  while (energy > 0) {
    move();
    eat(field);
    if (energy <= 0) break;
    std::this_thread::sleep_for(std::chrono::milliseconds(100));  // Simulate time delay
  }
}

void Cow::move() {
  position.x += rand() % 3 - 1;  // Random movement (-1, 0, 1)
  position.y += rand() % 3 - 1;
  energy -= 0.1f; // Spend energy to move
}

void Cow::eat(Grass& field) {
  // If the cow is on a grass spot, eat it (decrease grass, increase cow's energy)
  int x = static_cast<int>(position.x);
  int y = static_cast<int>(position.y);
  if (x >= 0 && x < field.getWidth() && y >= 0 && y < field.getHeight()) {
    int grass = field.getGrassAt(x, y);
    if (grass > 0) {
      energy += 10; // Cow eats grass to regain energy
      field.setGrassAt(x, y, grass - 5); // Decrease grass after being eaten
    }
  }
}

void Cow::render(sf::RenderWindow& window) {
  sf::CircleShape cowShape(5); // Circle shape for cow
  cowShape.setFillColor(color);
  cowShape.setPosition(position);
  window.draw(cowShape);
}

sf::Vector2f Cow::getPosition() const {
  return position;
}

void Cow::die() {
  // You can add logic here to remove or mark the cow as dead, for now we set energy to 0.
  energy = 0;
}

// Predator class methods
Predator::Predator() : energy(100), position(rand() % 800, rand() % 600), color(sf::Color(128, 128, 128)) {}

void Predator::huntAndMove(Grass& field, std::vector<Cow>& cows) {
  while (energy > 0) {
    hunt(cows);
    move();
    if (energy <= 0) break;
    std::this_thread::sleep_for(std::chrono::milliseconds(100));  // Simulate time delay
  }
}

void Predator::move() {
  position.x += rand() % 3 - 1;  // Random movement
  position.y += rand() % 3 - 1;
  energy -= 0.2f; // Spend energy to move
}

void Predator::hunt(std::vector<Cow>& cows) {
  for (auto& cow : cows) {
    // Check if predator is close to a cow, if yes, eat it
    if (abs(position.x - cow.getPosition().x) < 10 && abs(position.y - cow.getPosition().y) < 10) {
      energy += 30; // Predator eats cow to gain energy
      cow.die();    // Cow dies
      break;
    }
  }
}

void Predator::render(sf::RenderWindow& window) {
  sf::CircleShape predatorShape(5); // Circle shape for predator
  predatorShape.setFillColor(color);
  predatorShape.setPosition(position);
  window.draw(predatorShape);
}

sf::Vector2f Predator::getPosition() const {
  return position;
}

int main() {
  sf::RenderWindow window(sf::VideoMode(FIELD_WIDTH, FIELD_HEIGHT), "Life Simulation");

  // Create grass, cows, and predators
  Grass field(FIELD_WIDTH, FIELD_HEIGHT);
  std::vector<Cow> cows(NUM_COWS);
  std::vector<Predator> predators(NUM_PREDATORS);

  // Start thread for grass growth
  std::thread grassThread(&Grass::grow, &field);

  // Start a single thread for all cows
  std::thread cowsThread([&]() {
    for (auto& cow : cows) {
      cow.moveAndEat(field);  // Cows move and eat in a single thread
    }
    });

  // Start a single thread for all predators
  std::thread predatorsThread([&]() {
    for (auto& predator : predators) {
      predator.huntAndMove(field, cows);  // Predators hunt and move in a single thread
    }
    });

  // Main game loop
  while (window.isOpen()) {
    sf::Event event;
    while (window.pollEvent(event)) {
      if (event.type == sf::Event::Closed) {
        window.close();
      }
    }

    // Render everything
    window.clear();
    field.render(window);

    for (auto& cow : cows) {
      cow.render(window);
    }

    for (auto& predator : predators) {
      predator.render(window);
    }

    window.display();

    std::this_thread::sleep_for(std::chrono::milliseconds(16));  // Simulate ~60 FPS
  }

  // Join threads before exiting
  grassThread.join();
  cowsThread.join();
  predatorsThread.join();

  return 0;
}
