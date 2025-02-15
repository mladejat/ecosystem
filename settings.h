#pragma once
#include <stdint.h>

// Constants

constexpr float LIFE_LEVEL_MIN = 0.01f;
constexpr float LIFE_LEVEL_MAX = 4.0f;

constexpr int WINDOW_WIDTH = 800;
constexpr int WINDOW_HEIGHT = 600;
constexpr int CELL_SIZE = 20;
constexpr int GRID_WIDTH = WINDOW_WIDTH / CELL_SIZE;
constexpr int GRID_HEIGHT = WINDOW_HEIGHT / CELL_SIZE;
constexpr int INITIAL_COWS = 20;
constexpr int INITIAL_PREDATORS = 5;
constexpr float GRASS_GROWTH_RATE = 0.005f;

constexpr float MOVE_ENERGY_DECAY = 0.01f;

constexpr float COW_ENERGY_DECAY = 0.005f;
constexpr float COW_ENERGY_THRESHOLD = 1.5f;
constexpr float COW_VICINITY_RADIUS = 50.0f;
constexpr float COW_MAX_VIEW = 20.0f;
constexpr float COW_MAX_SPEED = 10.0f;

constexpr float PREDATOR_ENERGY_DECAY = 0.005f;
constexpr float PREDATOR_ENERGY_THRESHOLD = 4.0f;
constexpr float PREDATOR_MAX_VIEW = 10.0f;
constexpr float PREDATOR_MAX_SPEED = 10.0f;