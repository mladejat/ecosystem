/*
This program is a Visual Studio project in C++ (C17 standard) with a graphical user interface built using SFML-2.5.
It simulates interactions between three independent life forms: grass, cows, and predators within a rectangular field.
There are classes "Cow" and "Predator" and all existing objects are stored in 2 vectors.
Grass, cows, and predators are managed as three independent threads to simulate their interactions concurrently.
There are separate mutexes for grass, cows and predators.
Mutexes protect parameters changing of grass, cows and predators
Mutexes protect add/remove of cows and predators from vectors when they dies or reproduces.
If deal vectors with cycles as "for (auto& obj : objs)" or "for (it = objs.begin(); it != objs.end(); ++it)",
then if the vectors are modified in different threads it is possible to have an exception, because the iterator becomes invalid.
It is better to use "for" cycles in following form - "for(int i = 0; i < vect.size(); ++i)".

1. Grass:
   - Grows at a specific rate and is consumed by cows.
   - The growth rate is visually represented by the saturation of the green color. Taller grass appears as a more saturated green.
   - For increased program speed the field with grass is split of cells which are treated as one entity when it grow or is eaten by a cow.
   - Cows eats grass faster than it grows (for example cow eat whole grass in 3 cycles), so cows have to move to another grass cell.

2. Cows:
   - Represented as a red dots.
   - The initial number of cows is defined by a constant at the start of the program.
   - Cows gain energy(life) by grazing on grass.
   - Their energy level is indicated by the saturation of the red color.
   - Energy is spent by time, by movement and by reproduction.
   - When a cow’s energy(life) reaches 0, the cow dies and the object is removed from the vector.
   - When a cow’s energy reaches a certain threshold, it reproduces by splitting into two cows, each with the half of the energy.
   - Newly appeared object after reproduction is added to the vector.
   - Cows can "see" predators in some vicinity (defined as a constant in the beginning of the program).
   - Cows tries to escape the predator if one is nearby.
   - Cows eat grass when energy is low.
   - Cows reproduce when energy is sufficient.

3. Predators:
   - Represented as a gray dots.
   - The initial number of predators is defined by a constant at the start of the program.
   - Predators gain energy by eating cows.
   - Their energy(life) level is visually indicated by shades of gray, from light to dark, depending on energy.
   - Energy is spent by time, by movement and by reproduction.
   - When a predators’s energy(life) reaches 0, the predator dies and the object is removed from the vector.
   - When a predator’s energy reaches a certain threshold, it reproduces by splitting into two predators, each with the half of the energy.
   - Newly appeared object after reproduction is added to the vector.
   - When energy is low predators search for cows by detecting if nearby grass is eaten by the cow.

*/

#include <SFML/Graphics.hpp>
#include <vector>
#include <thread>
#include <mutex>
#include <random>
#include <chrono>
#include <cmath>
#include <iostream>
#include "settings..h"
#include "Cow.h"
#include "Predator.h"
#include "Utils.h"

std::mutex grass_mutex, cow_mutex, predator_mutex;



std::vector<Cow> cows;
std::vector<Predator> predators;
float grass[GRID_WIDTH][GRID_HEIGHT] = { 0 };

// Grass Thread
void grassThread()
{
  for (int x = 0; x < GRID_WIDTH; ++x)
  {
    for (int y = 0; y < GRID_HEIGHT; ++y)
    {
      grass[x][y] = 0.2f;
    }
  }
  while (true)
  {
    std::this_thread::sleep_for(std::chrono::milliseconds(150));

    std::lock_guard<std::mutex> lock(grass_mutex);
    for (int x = 0; x < GRID_WIDTH; ++x)
    {
      for (int y = 0; y < GRID_HEIGHT; ++y)
      {
        if (grass[x][y] < 1.0f)
        {
          grass[x][y] += GRASS_GROWTH_RATE;
        }
        else
        {
          grass[x][y] = 1.0f;
        }
      }
    }
  }
}

// Cow Thread
void cowThread()
{
  while (true)
  {
    std::this_thread::sleep_for(std::chrono::milliseconds(150));

    std::lock_guard<std::mutex> lock(cow_mutex);
    for (size_t i = 0; i < cows.size(); ++i)
    {
      cows[i].move();

      // Eat grass
      int gridX = static_cast<int>(cows[i].position.x) / CELL_SIZE;
      int gridY = static_cast<int>(cows[i].position.y) / CELL_SIZE;
      if (gridX >= 0 && gridX < GRID_WIDTH && gridY >= 0 && gridY < GRID_HEIGHT)
      {
        std::lock_guard<std::mutex> grass_lock(grass_mutex);
        for (int j = 0; j < 5; ++j)
        {
          if (grass[gridX][gridY] > 0.2f)
          {
            grass[gridX][gridY] -= 0.2f;
            cows[i].energy += 0.2f;
            break;
          }
          else
          {
            cows[i].move();
          }
        }
      }

      // Reproduction
      if (cows[i].energy >= COW_ENERGY_THRESHOLD)
      {
        cows[i].energy /= 2;
        cows.emplace_back(cows[i].position.x + 5, cows[i].position.y + 5);
      }

      // Death
      if (cows[i].energy <= 0.0f)
      {
        cows.erase(cows.begin() + i);
        --i;
        std::cout << "cow day" << std::endl;
      }
    }
  }
}

// Predator Thread
void predatorThread()
{
  while (true)
  {
    std::this_thread::sleep_for(std::chrono::milliseconds(150));

    std::lock_guard<std::mutex> lock(predator_mutex);
    for (size_t i = 0; i < predators.size(); ++i)
    {
      predators[i].move();

      // Hunt cows
      std::lock_guard<std::mutex> cow_lock(cow_mutex);
      for (size_t j = 0; j < cows.size(); ++j)
      {
        if (std::hypot(predators[i].position.x - cows[j].position.x, predators[i].position.y - cows[j].position.y) < 10.0f)
        {
          predators[i].energy += 0.5f;
          cows.erase(cows.begin() + j);
          --j;
        }
      }

      // Reproduction
      if (predators[i].energy >= PREDATOR_ENERGY_THRESHOLD)
      {
        predators[i].energy /= 2;
        predators.emplace_back(predators[i].position.x + 5, predators[i].position.y + 5);
      }

      // Death
      if (predators[i].energy <= 0.0f)
      {
        predators.erase(predators.begin() + i);
        --i;
      }
    }
  }
}

int main()
{
  sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Life Simulation");

  std::cout << "std::thread::hardware_concurrency():" << std::thread::hardware_concurrency() << std::endl;

  // Initialize cows and predators
  for (int i = 0; i < INITIAL_COWS; ++i)
  {
    cows.emplace_back(Utils::getRandomFloat(0, WINDOW_WIDTH), Utils::getRandomFloat(0, WINDOW_HEIGHT));
  }

  for (int i = 0; i < INITIAL_PREDATORS; ++i)
  {
    predators.emplace_back(Utils::getRandomFloat(0, WINDOW_WIDTH), Utils::getRandomFloat(0, WINDOW_HEIGHT));
  }

  // Start threads
  std::thread grass_thread(grassThread);
  std::thread cow_thread(cowThread);
  std::thread predator_thread(predatorThread);

  while (window.isOpen())
  {
    sf::Event event;
    while (window.pollEvent(event))
    {
      if (event.type == sf::Event::Closed)
      {
        window.close();
      }
    }

    window.clear();

    // Draw grass
    grass_mutex.lock();
    for (int x = 0; x < GRID_WIDTH; ++x)
    {
      for (int y = 0; y < GRID_HEIGHT; ++y)
      {
        if (grass[x][y] > 0)
        {
          sf::RectangleShape cell(sf::Vector2f(CELL_SIZE, CELL_SIZE));
          cell.setPosition(x * CELL_SIZE, y * CELL_SIZE);
          cell.setFillColor(sf::Color(0, static_cast<int>(grass[x][y] * 255), 0));
          window.draw(cell);
        }
      }
    }
    grass_mutex.unlock();

    // Draw cows
    cow_mutex.lock();
    for (auto& cow : cows)
    {
      cow.draw(window);
    }
    cow_mutex.unlock();

    // Draw predators
    predator_mutex.lock();
    for (auto& predator : predators)
    {
      predator.draw(window);
    }
    predator_mutex.unlock();

    window.display();
  }

  // Join threads
  grass_thread.join();
  cow_thread.join();
  predator_thread.join();

  return 0;
}

