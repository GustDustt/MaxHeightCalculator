#include <cmath>
#include <iostream>
#include <stdexcept>
#include <vector>

using namespace std;
double Calculate_Density_max(double m, double D, double N, double RPM, double Ct0);
double Calculate_Density_max2(double Density_run, double W, double N, double T_run);
double Calculate_height(double Density_max);

// Physics constants

const double g = 9.80665;
const double Density_0 = 1.225;
const double T0 = 288.15;
const double L1 = 0.0065;
const double e = 0.23497422732018655;
const double boundary = 11000;
const double T1 = 216.65;
const double R = 287.05287;

int main() {

  double Density_max, N, D, Ct0, Density_run, T_run, m, RPM, choice;

  cout << "Which calculation way do you want to  use? (1 - Density_max without Density_run) (2 - Density_max with Density_run)" << endl;
  cin >> choice;

  if (choice == 1) {
    cout << "Mass (kg): " << endl;
    cin >> m;
    cout << "Number of propellers: " << endl;
    cin >> N;
    cout << "Propeller diameter (in meters): " << endl;
    cin >> D;
    cout << "RPM of the propellers: " << endl;
    cin >> RPM;
    cout << "Static thrust coefficient when J = 0: " << endl;
    cin >> Ct0;

    Density_max = Calculate_Density_max(m, D, N, RPM, Ct0);
  }

  /* If you already ran QBlade at density 𝜌run (e.g. sea level) and exported
    per-rotor thrust Trun, you can use the following equation to find th
    maximum density 𝜌max:
  */  

  if (choice == 2) {
    cout << "Mass (kg): " << endl;
    cin >> m;
    cout << "Number of propellers: " << endl;
    cin >> N;
    cout << "Static thrust, which you calculated or got from QBlade: " << endl;
    cin >> T_run;
    cout << "Density, which you used for the static thrust calculation: " << endl;
    cin >> Density_run;

    Density_max = Calculate_Density_max2(Density_run, m, N, T_run);
  }

  cout << "Density : " << Density_max << " kg/m^3" << endl;

  cout << "Max height will be: " << Calculate_height(Density_max) << " m" << endl;
};

double Calculate_Density_max(double m, double D, double N, double RPM, double Ct0) {

  double n = RPM / 60;
  double W = m * g;
  double Density_max = W / (N * pow(n, 2.0) * pow(D, 4.0) * Ct0);

  return (Density_max);
};

double Calculate_Density_max2(double Density_run, double m, double N, double T_run) {

  double W = m * g;
  double Density_max = Density_run * (W / (N * T_run));
  return (Density_max);
}

double Calculate_height(double Density_max) {
    if (!(Density_max > 0.0))
        throw std::invalid_argument("Density_max must be positive");

    // Sea-level air density
    const double Density_0 = 1.225;

    // If density is greater than sea-level, max altitude = 0
    if (Density_max >= Density_0)
        return 0.0;

    // ISA layer base heights (m)
    const std::vector<double> h_base = {
        0.0, 11000.0, 20000.0, 32000.0, 47000.0,
        51000.0, 71000.0, 84852.0, 100000.0
    };

    // Corresponding lapse rates (K/m)
    const std::vector<double> L = {
        -0.0065, 0.0, 0.0010, 0.0028, 0.0, -0.0028, -0.0020, 0.0
    };

    const size_t n_layers = L.size();

    // Precompute base temperatures for each layer
    std::vector<double> T_base(n_layers + 1);
    T_base[0] = T0;

    for (size_t i = 0; i < n_layers; ++i) {
        T_base[i + 1] = T_base[i] + L[i] * (h_base[i + 1] - h_base[i]);
    }

    // Precompute densities at layer bases using simple ISA formula
    std::vector<double> Density_base(n_layers + 1);
    Density_base[0] = Density_0;

    for (size_t i = 0; i < n_layers; ++i) {
        double Li = L[i];
        double T_i = T_base[i];
        double rho_i = Density_base[i];
        double h_i = h_base[i];
        double h_next = h_base[i + 1];

        if (Li == 0.0) {
            // isothermal layer
            Density_base[i + 1] = rho_i * std::exp(-g * (h_next - h_i) / (R * T_i));
        } else {
            // non-zero lapse rate
            double exponent = g / (R * Li) - 1.0;
            Density_base[i + 1] = rho_i * std::pow(T_i / T_base[i + 1], exponent);
        }
    }

    // Find the layer where Density_max falls
    size_t layer = 0;
    bool found = false;
    for (size_t k = 0; k < n_layers; ++k) {
        if (Density_max <= Density_base[k] && Density_max >= Density_base[k + 1]) {
            layer = k;
            found = true;
            break;
        }
    }

    // If not found, it's below the last layer → extrapolate top layer
    if (!found) {
        layer = n_layers - 1;
    }

    // Invert density formula in the found layer
    double h0_layer = h_base[layer];
    double T0_layer = T_base[layer];
    double Density_0_layer = Density_base[layer];
    double Li = L[layer];

    double h_out = 0.0;
    if (Li == 0.0) {
        // isothermal inversion
        h_out = h0_layer - (R * T0_layer / g) * std::log(Density_max / Density_0_layer);
    } else {
        // non-zero lapse inversion
        double exponent = 1.0 / (g / (R * Li) - 1.0);
        double factor = std::pow(Density_0_layer / Density_max, exponent);
        h_out = h0_layer + (T0_layer / Li) * (factor - 1.0);
    }

    // Safety clamp
    if (h_out < 0.0) h_out = 0.0;
    if (h_out > 100000.0) h_out = 100000.0;

    return h_out;
}
