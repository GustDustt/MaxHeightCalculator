#ifndef MAX_HEIGHT_CALCULATOR_HPP
#define MAX_HEIGHT_CALCULATOR_HPP

#include <vector>

// Constants
constexpr double GRAVITY = 9.80665;
constexpr double R_GAS = 287.05287;
constexpr double SEA_LEVEL_DENSITY = 1.225;
constexpr double SEA_LEVEL_TEMP = 288.15;

// Function declarations
double calculateDensityMax(double mass, double propDiameter, double propCount, double rpm, double thrustCoeff);
double calculateDensityMaxFromRun(double densityRun, double mass, double propCount, double thrustPerRotor);
double calculateHeight(double densityMax);

#endif
