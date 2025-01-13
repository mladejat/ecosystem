#pragma once
#include <random>
#include "settings..h"

class Utils
{
public:
  static float getRandomFloat(float min, float max)
  {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_real_distribution<float> dis(min, max);
    return dis(gen);
  }
};

