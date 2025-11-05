#include "MaxHeightCalculator.hpp"
#include <cmath>
#include <stdexcept>
#include <vector>

// Calculates max density using easy to get inputs
double calculateDensityMax(double mass, double propDiameter, double propCount, double rpm, double thrustCoeff) {
    double n = rpm / 60.0;
    double weight = mass * GRAVITY;
    return weight / (propCount * std::pow(n, 2.0) * std::pow(propDiameter, 4.0) * thrustCoeff);
}

// Calculates max density using inputs you would need to calculate yourself
double calculateDensityMaxFromRun(double densityRun, double mass, double propCount, double thrustPerRotor) {
    double weight = mass * GRAVITY;
    return densityRun * (weight / (propCount * thrustPerRotor));
}


// Calculates max height using the max density the drone could reach, and putting into a specific ISA layer
double calculateHeight(double densityMax) {
    if (densityMax <= 0.0)
        throw std::invalid_argument("Density must be positive.");

    if (densityMax >= SEA_LEVEL_DENSITY)
        return 0.0;

    // ISA layer base heights (m)
    const std::vector<double> h_base = {0, 11000, 20000, 32000, 47000, 51000, 71000, 84852, 100000};
    const std::vector<double> lapse = {-0.0065, 0.0, 0.0010, 0.0028, 0.0, -0.0028, -0.0020, 0.0};

    std::vector<double> temp_base(lapse.size() + 1);
    temp_base[0] = SEA_LEVEL_TEMP;
    for (size_t i = 0; i < lapse.size(); ++i)
        temp_base[i + 1] = temp_base[i] + lapse[i] * (h_base[i + 1] - h_base[i]);

    std::vector<double> rho_base(lapse.size() + 1);
    rho_base[0] = SEA_LEVEL_DENSITY;
    for (size_t i = 0; i < lapse.size(); ++i) {
        if (lapse[i] == 0.0)
            rho_base[i + 1] = rho_base[i] * std::exp(-GRAVITY * (h_base[i + 1] - h_base[i]) / (R_GAS * temp_base[i]));
        else {
            double expTerm = GRAVITY / (R_GAS * lapse[i]) - 1.0;
            rho_base[i + 1] = rho_base[i] * std::pow(temp_base[i] / temp_base[i + 1], expTerm);
        }
    }

    // Find layer
    size_t layer = 0;
    for (size_t i = 0; i < lapse.size(); ++i)
        if (densityMax <= rho_base[i] && densityMax >= rho_base[i + 1])
            layer = i;

    double Li = lapse[layer];
    double T0_layer = temp_base[layer];
    double rho0_layer = rho_base[layer];
    double h0_layer = h_base[layer];

    double h_out;
    if (Li == 0.0)
        h_out = h0_layer - (R_GAS * T0_layer / GRAVITY) * std::log(densityMax / rho0_layer);
    else {
        double expTerm = 1.0 / (GRAVITY / (R_GAS * Li) - 1.0);
        double factor = std::pow(rho0_layer / densityMax, expTerm);
        h_out = h0_layer + (T0_layer / Li) * (factor - 1.0);
    }

    if (h_out < 0.0) h_out = 0.0;
    if (h_out > 100000.0) h_out = 100000.0;

    return h_out;
}
