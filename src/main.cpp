#include "MaxHeightCalculator.hpp"
#include <iostream>
#include <iomanip>

int main() {
    std::cout << "=== Max Height Calculator ===\n";
    std::cout << "1 - Use theoretical propeller data\n";
    std::cout << "2 - Use measured thrust data\n> ";

    int choice;
    std::cin >> choice;

    double densityMax = 0.0;

    try {
        if (choice == 1) {
            double mass, propCount, propDiameter, rpm, thrustCoeff;
            std::cout << "Mass (kg): "; std::cin >> mass;
            std::cout << "Propeller count: "; std::cin >> propCount;
            std::cout << "Propeller diameter (m): "; std::cin >> propDiameter;
            std::cout << "RPM: "; std::cin >> rpm;
            std::cout << "Static thrust coefficient (Ct0): "; std::cin >> thrustCoeff;

            densityMax = calculateDensityMax(mass, propDiameter, propCount, rpm, thrustCoeff);
        } else if (choice == 2) {
            double mass, propCount, thrustPerRotor, densityRun;
            std::cout << "Mass (kg): "; std::cin >> mass;
            std::cout << "Propeller count: "; std::cin >> propCount;
            std::cout << "Thrust per rotor (N): "; std::cin >> thrustPerRotor;
            std::cout << "Density used in thrust test (kg/m³): "; std::cin >> densityRun;

            densityMax = calculateDensityMaxFromRun(densityRun, mass, propCount, thrustPerRotor);
        } else {
            std::cerr << "Invalid choice.\n";
            return 1;
        }

        double height = calculateHeight(densityMax);

        std::cout << std::fixed << std::setprecision(3);
        std::cout << "\nAir density at max height: " << densityMax << " kg/m³\n";
        std::cout << "Maximum theoretical altitude: " << height << " m\n";
    }
    catch (const std::exception &e) {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    }
}
