#include <cmath>
#include <iostream>
#include <stdexcept>
#include <vector>

using namespace std;
double Skaiciav_Tankis_max(double m, double D, double N, double RPM,
                           double Ct0);
double Skaiciav_Tankis_max2(double Tankis_run, double W, double N,
                            double T_run);
double Skaiciav_aukstis(double Tankis_max);

const double g = 9.80665;
const double Tankis_0 = 1.225;
const double T0 = 288.15;
const double L1 = 0.0065;
const double e = 0.23497422732018655;
const double boundary = 11000;
const double T1 = 216.65;
const double R = 287.05287;

int main() {

  double Tankis_max, N, D, Ct0, Tankis_run, T_run, m, RPM, pasirinkimas;

  cout << "Kurio norite skaiciavimo? (1 - Tankis_max be Tankis_run) (2 - "
          "Tankis_max su Tankis_run)"
       << endl;
  cin >> pasirinkimas;

  if (pasirinkimas == 1) {
    cout << "Iveskite mase (kg): " << endl;
    cin >> m;
    cout << "Iveskite kieki propeleriu: " << endl;
    cin >> N;
    cout << "Iveskite propelerio diametra (m): " << endl;
    cin >> D;
    cout << "Iveskite propelerio pasukimu skaiciu (RPM): " << endl;
    cin >> RPM;
    cout << "Iveskite statinės traukos koeficienta, kai J = 0 (poslinkio "
            "laipsnis): "
         << endl;
    cin >> Ct0;

    Tankis_max = Skaiciav_Tankis_max(m, D, N, RPM, Ct0);
  }

  // If you already ran QBlade at density 𝜌run (e.g. sea level) and exporte
  //  per-rotor thrust Trun, you can use the following equation to find th
  //  maximum density 𝜌max:

  if (pasirinkimas == 2) {
    cout << "Iveskite mase (kg): " << endl;
    cin >> m;
    cout << "Iveskite kieki propeleriu: " << endl;
    cin >> N;
    cout << "Iveskite T_run, kuri gavai is QBlade: " << endl;
    cin >> T_run;
    cout << "Iveskite Tankis_run, kuri irasei i QBlade (geriau sea level "
            "1.225): "
         << endl;
    cin >> Tankis_run;

    Tankis_max = Skaiciav_Tankis_max2(Tankis_run, m, N, T_run);
  }

  cout << "Tankis yra: " << Tankis_max << " kg/m^3" << endl;

  cout << "Maximalus aukstis bus: " << Skaiciav_aukstis(Tankis_max) << " m"
       << endl;
};

double Skaiciav_Tankis_max(double m, double D, double N, double RPM,
                           double Ct0) {

  double n = RPM / 60;
  double W = m * g;
  double Tankis_max = W / (N * pow(n, 2.0) * pow(D, 4.0) * Ct0);

  return (Tankis_max);
};

double Skaiciav_Tankis_max2(double Tankis_run, double m, double N,
                            double T_run) {

  double W = m * g;
  double Tankis_max = Tankis_run * (W / (N * T_run));
  return (Tankis_max);
}

double Skaiciav_aukstis(double Tankis_max) {
    if (!(Tankis_max > 0.0))
        throw std::invalid_argument("Tankis_max must be positive");

    // Sea-level air density
    const double Tankis_0 = 1.225;

    // If density is greater than sea-level, max altitude = 0
    if (Tankis_max >= Tankis_0)
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
    std::vector<double> Tankis_base(n_layers + 1);
    Tankis_base[0] = Tankis_0;

    for (size_t i = 0; i < n_layers; ++i) {
        double Li = L[i];
        double T_i = T_base[i];
        double rho_i = Tankis_base[i];
        double h_i = h_base[i];
        double h_next = h_base[i + 1];

        if (Li == 0.0) {
            // isothermal layer
            Tankis_base[i + 1] = rho_i * std::exp(-g * (h_next - h_i) / (R * T_i));
        } else {
            // non-zero lapse rate
            double exponent = g / (R * Li) - 1.0;
            Tankis_base[i + 1] = rho_i * std::pow(T_i / T_base[i + 1], exponent);
        }
    }

    // Find the layer where Tankis_max falls
    size_t layer = 0;
    bool found = false;
    for (size_t k = 0; k < n_layers; ++k) {
        if (Tankis_max <= Tankis_base[k] && Tankis_max >= Tankis_base[k + 1]) {
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
    double Tankis_0_layer = Tankis_base[layer];
    double Li = L[layer];

    double h_out = 0.0;
    if (Li == 0.0) {
        // isothermal inversion
        h_out = h0_layer - (R * T0_layer / g) * std::log(Tankis_max / Tankis_0_layer);
    } else {
        // non-zero lapse inversion
        double exponent = 1.0 / (g / (R * Li) - 1.0);
        double factor = std::pow(Tankis_0_layer / Tankis_max, exponent);
        h_out = h0_layer + (T0_layer / Li) * (factor - 1.0);
    }

    // Safety clamp
    if (h_out < 0.0) h_out = 0.0;
    if (h_out > 100000.0) h_out = 100000.0;

    return h_out;
}
