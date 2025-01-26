#pragma once
#include <stdint.h>

// Constants

const uint32_t maxHerbivoreView = 3;
const uint32_t maxPredatorView = 4;
const uint32_t maxHerbivoreSpeed = 3;
const uint32_t maxPredatorSpeed = 2;
const float minLifeLevel = 0.01;
const float maxLifeLevel = 10.0;

constexpr int WINDOW_WIDTH = 800;
constexpr int WINDOW_HEIGHT = 600;
constexpr int CELL_SIZE = 20;
constexpr int GRID_WIDTH = WINDOW_WIDTH / CELL_SIZE;
constexpr int GRID_HEIGHT = WINDOW_HEIGHT / CELL_SIZE;
constexpr int INITIAL_COWS = 10;
constexpr int INITIAL_PREDATORS = 5;
constexpr float GRASS_GROWTH_RATE = 0.005f;
constexpr float COW_ENERGY_THRESHOLD = 1.5f;
constexpr float PREDATOR_ENERGY_THRESHOLD = 2.0f;
constexpr float COW_VICINITY_RADIUS = 50.0f;
constexpr float PREDATOR_ENERGY_DECAY = 0.005f;
constexpr float COW_ENERGY_DECAY = 0.005f;