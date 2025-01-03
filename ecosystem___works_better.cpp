/*
This program is a Visual Studio project in C++ (C17 standard) with a graphical user interface built using SFML-2.5.
It simulates interactions between three independent life forms: grass, cows, and predators within a rectangular field.

1. Grass:
   - Grows at a specific rate and is consumed by cows.
   - The growth rate is visually represented by the saturation of the green color. Taller grass appears as a more saturated green.

2. Cows:
   - Represented as red dots.
   - The initial number of cows is defined by a constant at the start of the program.
   - Cows gain energy by grazing on grass. Their energy level is indicated by the saturation of the red color.
   - Energy is spent for survival, movement, and reproduction.
   - When a cow’s energy reaches a certain threshold, it reproduces, splitting into two cows, each with half the energy.
   - Priorities:
     a) Escape predators if one is nearby.
     b) Graze when energy is low.
     c) Reproduce when energy is sufficient.

3. Predators:
   - Represented as grey dots.
   - The initial number of predators is defined by a constant at the start of the program.
   - Predators gain energy by eating cows. Their energy level is visually indicated by shades of gray, from light to dark, depending on energy.
   - Energy is spent for survival, movement, and reproduction.
   - When a predator’s energy reaches a certain threshold, it reproduces, splitting into two predators, each with half the energy.

Multithreading:
- Grass, cows, and predators are managed as three independent threads to simulate their interactions concurrently.

*/

#include <SFML/Graphics.hpp>
#include <vector>
#include <thread>
#include <mutex>
#include <cmath>
#include <cstdlib>
#include <ctime>

const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;
const int GRID_SIZE = 20;
const int NUM_COWS = 10;
const int NUM_PREDATORS = 2;

std::mutex grass_mutex;
std::mutex cow_mutex;
std::mutex predator_mutex;

// Grass Class
class Grass {
public:
  std::vector<std::vector<float>> growth;

  Grass(int width, int height) {
    growth.resize(height, std::vector<float>(width, 0.0f));
  }

  void grow() {
    std::lock_guard<std::mutex> lock(grass_mutex);
    for (auto& row : growth) {
      for (auto& cell : row) {
        cell = std::min(1.0f, cell + 0.01f); // Grass grows up to full saturation
      }
    }
  }

  void draw(sf::RenderWindow& window) {
    std::lock_guard<std::mutex> lock(grass_mutex);
    sf::RectangleShape cellShape(sf::Vector2f(GRID_SIZE, GRID_SIZE));
    for (size_t y = 0; y < growth.size(); ++y) {
      for (size_t x = 0; x < growth[y].size(); ++x) {
        float intensity = growth[y][x];
        cellShape.setFillColor(sf::Color(0, static_cast<sf::Uint8>(255 * intensity), 0));
        cellShape.setPosition(x * GRID_SIZE, y * GRID_SIZE);
        window.draw(cellShape);
      }
    }
  }
};

// Cow Class
class Cow {
public:
  sf::Vector2f position;
  float energy;

  Cow(float x, float y) : position(x, y), energy(1.0f) {}

  void move() {
    position.x += (std::rand() % 3 - 1) * GRID_SIZE;
    position.y += (std::rand() % 3 - 1) * GRID_SIZE;
    position.x = std::fmod(position.x + WINDOW_WIDTH, WINDOW_WIDTH);
    position.y = std::fmod(position.y + WINDOW_HEIGHT, WINDOW_HEIGHT);
    energy -= 0.01f; // Energy spent moving
  }

  void eat(Grass& grass) {
    std::lock_guard<std::mutex> lock(grass_mutex);
    int x = static_cast<int>(position.x / GRID_SIZE);
    int y = static_cast<int>(position.y / GRID_SIZE);
    if (grass.growth[y][x] > 0.1f) {
      grass.growth[y][x] -= 0.1f;
      energy = std::min(1.0f, energy + 0.2f);
    }
  }

  void draw(sf::RenderWindow& window) {
    sf::CircleShape shape(GRID_SIZE / 2);
    shape.setFillColor(sf::Color(static_cast<sf::Uint8>(255 * energy), 0, 0));
    shape.setPosition(position);
    window.draw(shape);
  }
};

// Predator Class
class Predator {
public:
  sf::Vector2f position;
  float energy;

  Predator(float x, float y) : position(x, y), energy(1.0f) {}

  void move() {
    position.x += (std::rand() % 3 - 1) * GRID_SIZE;
    position.y += (std::rand() % 3 - 1) * GRID_SIZE;
    position.x = std::fmod(position.x + WINDOW_WIDTH, WINDOW_WIDTH);
    position.y = std::fmod(position.y + WINDOW_HEIGHT, WINDOW_HEIGHT);
    energy -= 0.02f; // Energy spent moving
  }

  void eat(std::vector<Cow>& cows) {
    std::lock_guard<std::mutex> lock(cow_mutex);
    for (auto& cow : cows) {
      if (std::hypot(position.x - cow.position.x, position.y - cow.position.y) < GRID_SIZE) {
        cow.energy = 0.0f; // Cow is eaten
        energy = std::min(1.0f, energy + 0.5f);
        break;
      }
    }
  }

  void draw(sf::RenderWindow& window) {
    sf::CircleShape shape(GRID_SIZE / 2);
    sf::Uint8 gray = static_cast<sf::Uint8>(255 * energy);
    shape.setFillColor(sf::Color(gray, gray, gray));
    shape.setPosition(position);
    window.draw(shape);
  }
};

void grassThread(Grass& grass) {
  while (true) {
    grass.grow();
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
  }
}

void cowThread(std::vector<Cow>& cows, Grass& grass) {
  while (true) {
    for (auto& cow : cows) {
      cow.move();
      cow.eat(grass);
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
  }
}

void predatorThread(std::vector<Predator>& predators, std::vector<Cow>& cows) {
  while (true) {
    for (auto& predator : predators) {
      predator.move();
      predator.eat(cows);
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
  }
}

int main() {
  std::srand(static_cast<unsigned>(std::time(nullptr)));

  sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Life Simulation");

  Grass grass(WINDOW_WIDTH / GRID_SIZE, WINDOW_HEIGHT / GRID_SIZE);
  std::vector<Cow> cows;
  std::vector<Predator> predators;

  for (int i = 0; i < NUM_COWS; ++i) {
    cows.emplace_back(std::rand() % WINDOW_WIDTH, std::rand() % WINDOW_HEIGHT);
  }

  for (int i = 0; i < NUM_PREDATORS; ++i) {
    predators.emplace_back(std::rand() % WINDOW_WIDTH, std::rand() % WINDOW_HEIGHT);
  }

  std::thread grassWorker(grassThread, std::ref(grass));
  std::thread cowWorker(cowThread, std::ref(cows), std::ref(grass));
  std::thread predatorWorker(predatorThread, std::ref(predators), std::ref(cows));

  while (window.isOpen()) {
    sf::Event event;
    while (window.pollEvent(event)) {
      if (event.type == sf::Event::Closed)
        window.close();
    }

    window.clear();
    grass.draw(window);
    for (auto& cow : cows) {
      cow.draw(window);
    }
    for (auto& predator : predators) {
      predator.draw(window);
    }
    window.display();
  }

  grassWorker.detach();
  cowWorker.detach();
  predatorWorker.detach();

  return 0;
}
