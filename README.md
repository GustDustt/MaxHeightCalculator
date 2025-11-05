# 🛩️ MaxHeightCalculator

**MaxHeightCalculator** is a C++ program that estimates the *maximum theoretical altitude* a drone or propeller-driven vehicle can reach using aerodynamic and atmospheric models (NASA/ISA-based).

---

## 🚀 Features
- Calculates max altitude using either:
  - **Theoretical propeller data**, or
  - **Measured thrust data**
- Implements the full **ISA (International Standard Atmosphere)** model up to 100 km
- Validates user inputs and handles both isothermal and gradient layers

---

## ⚙️ How to Use

### 🧩 Compile
```bash
g++ src/main.cpp src/MaxHeightCalculator.cpp -o MaxHeightCalculator
```

### ▶️ Run
./MaxHeightCalculator

### Nasa proof link

https://ntrs.nasa.gov/api/citations/19830024539/downloads/19830024539.pdf
