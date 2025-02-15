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
#include "settings.h"
#include "GrassCell.h"
#include "Cow.h"
#include "Predator.h"
#include "Utils.h"

std::mutex mtx;// grass_mutex, cow_mutex, predator_mutex;
//!!! Use only one shared_mutex and unique_lock(when write or add/delete element) + shared_lock(when only read)


std::vector<Cow> cows;
std::vector<Predator> predators;
GrassCell field[GRID_WIDTH][GRID_HEIGHT];

// Grass Thread
void grassThread()
{
  for (uint32_t x = 0; x < GRID_WIDTH; ++x)
  {
    for (uint32_t y = 0; y < GRID_HEIGHT; ++y)
    {
      field[x][y] = { static_cast<float>(x), static_cast<float>(y), LIFE_LEVEL_MAX };
    }
  }
  while (true)
  {
    std::this_thread::sleep_for(std::chrono::milliseconds(50));

    std::lock_guard<std::mutex> lock(mtx);
    for (uint32_t x = 0; x < GRID_WIDTH; ++x)
    {
      for (uint32_t y = 0; y < GRID_HEIGHT; ++y)
      {
        field[x][y].growUp();
      }
    }
  }
}

// Cow Thread
void cowThread()
{
  while (true)
  {
    std::this_thread::sleep_for(std::chrono::milliseconds(50));

    std::lock_guard<std::mutex> lock(mtx);

    auto size = cows.size();
    std::cout << "cows.size():" << size << "\n";
    if (size)
    {
      for (size_t i = 0; i < cows.size(); ++i)
      {
        std::cout << "cowThread - cows:" << cows.size() << ", cow:" << i << " - findClosest predator from " << predators.size() << "\n";
        auto [closestPred, dist] = cows[i].findClosest(predators);
        if (dist <= COW_MAX_VIEW)
        {
          cows[i].moveAway(closestPred);
        }
        else
        {
          // Eat grass
          cows[i].moveToward(cows[i].searchHighGrass(field));
          cows[i].setLifeLevel(cows[i].getLifeLevel() + field[static_cast<int>(cows[i].getX()) / CELL_SIZE][static_cast<int>(cows[i].getY()) / CELL_SIZE].eatGrass());
        }

        cows[i].setLifeLevel(cows[i].getLifeLevel() - COW_ENERGY_DECAY);

        // Reproduction
        if (cows[i].getLifeLevel() >= COW_ENERGY_THRESHOLD)
        {
          std::cout << "cow:" << i << " reproduces\n";
          cows[i].setLifeLevel(cows[i].getLifeLevel() / 2);
          //cows.emplace_back(cows[i].getX() + Utils::getRandomFloat(-5.0f, 5.0f), cows[i].getY() + Utils::getRandomFloat(-5.0f, 5.0f));
          cows.insert(cows.begin() + i + 1, { cows[i].getX() + Utils::getRandomFloat(-5.0f, 5.0f), cows[i].getY() + Utils::getRandomFloat(-5.0f, 5.0f) });
          ++i;
        }
        else if (cows[i].getLifeLevel() <= 0.0f) // Death
        {
          std::cout << "cow:" << i << " day\n";
          cows.erase(cows.begin() + i);
          --i;
        }
      }
    }
  }
}

// Predator Thread
void predatorThread()
{
  while (true)
  {
    std::this_thread::sleep_for(std::chrono::milliseconds(50));

    std::lock_guard<std::mutex> lock(mtx);

    auto size = predators.size();
    std::cout << "predators.size():" << size << "\n";
    if (size)
    {
      for (size_t i = 0; i < predators.size(); ++i)
      {
        std::cout << "predatorThread - predators:" << predators.size() << ", predator:" << i << "\n";

        auto [closestCaw, dist] = predators[i].findClosest(cows);
        predators[i].moveToward(closestCaw);

        // Hunt cows
        for (size_t j = 0; j < cows.size(); ++j)
        {
          if (std::hypot(predators[i].getX() - cows[j].getX(), predators[i].getY() - cows[j].getY()) < 10.0f)
          {
            std::cout << "predator eat cow:" << j << "\n";
            predators[i].setLifeLevel(predators[i].getLifeLevel() + 0.5f);
            cows.erase(cows.begin() + j);
            --j;
          }
        }

        // Reproduction
        if (predators[i].getLifeLevel() >= PREDATOR_ENERGY_THRESHOLD)
        {
          std::cout << "predator reproduces\n";
          predators[i].setLifeLevel(predators[i].getLifeLevel() / 2);
          //predators.emplace_back(predators[i].getX() + Utils::getRandomFloat(-5.0f, 5.0f), predators[i].getY() + Utils::getRandomFloat(-5.0f, 5.0f));
          predators.insert(predators.begin() + i + 1, { predators[i].getX() + Utils::getRandomFloat(-5.0f, 5.0f), predators[i].getY() + Utils::getRandomFloat(-5.0f, 5.0f) });
          ++i;
        }
        else if (predators[i].getLifeLevel() <= 0.0f)
        {
          std::cout << "predator day\n";
          predators.erase(predators.begin() + i);
          --i;
        }
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

    std::this_thread::sleep_for(std::chrono::milliseconds(50));

    window.clear();

    // Draw grass
    mtx.lock();
    for (int x = 0; x < GRID_WIDTH; ++x)
    {
      for (int y = 0; y < GRID_HEIGHT; ++y)
      {
        if (field[x][y].getLifeLevel() > 0.0f)
        {
          sf::RectangleShape cell(sf::Vector2f(CELL_SIZE, CELL_SIZE));
          cell.setPosition(x * CELL_SIZE, y * CELL_SIZE);
          cell.setFillColor(sf::Color(0, static_cast<int>(field[x][y].getLifeLevel() / LIFE_LEVEL_MAX * 255), 0));
          window.draw(cell);
        }
      }
    }

    std::cout << "dddddddddddddddddddddddraw, cows.size():" << cows.size() << "\n";
    //for (auto& cow : cows)
    for (size_t i = 0; i < cows.size(); ++i)
    {
      cows[i].draw(window);
    }
    
    std::cout << "dddddddddddddddddddddddraw, predators.size():" << predators.size() << "\n";
    //for (auto& predator : predators)
    for (size_t i = 0; i < predators.size(); ++i)
    {
      predators[i].draw(window);
    }
    mtx.unlock();

    window.display();
  }

  // Join threads
  grass_thread.join();
  cow_thread.join();
  predator_thread.join();

  return 0;
}

