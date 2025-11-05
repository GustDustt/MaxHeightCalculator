# 🛩️ MaxHeightCalculator

**MaxHeightCalculator** is a C++ program that estimates the **maximum theoretical altitude** a drone or propeller-driven vehicle can reach using aerodynamic and atmospheric models based on NASA and ISA standards.

---

[![CMake Build](https://img.shields.io/badge/build-CMake-blue.svg)](#)
[![License: MIT](https://img.shields.io/badge/license-MIT-green.svg)](#)
[![Language: C++17](https://img.shields.io/badge/language-C%2B%2B17-red.svg)](#)

---

## 🚀 Features

- Calculates maximum altitude using either:
  - **Theoretical propeller data**, or  
  - **Measured thrust data**
- Implements the full **International Standard Atmosphere (ISA)** model up to **100 km**
- Validates user inputs and handles both **isothermal** and **gradient** layers
- Written in modern **C++17** with a clean modular structure

---

## ⚙️ How to Build and Run

### 🧩 Option 1 — Build with CMake (recommended)

This project supports **CMake**, so you can easily build it on any platform.

```bash
# 1️⃣ Open a terminal in the project folder
cd MaxHeightCalculator

# 2️⃣ Generate build files
cmake .

# 3️⃣ Compile the project
make

# 4️⃣ Run the program
./MaxHeightCalculator

```

You’ll find the compiled executable in the same folder.

### 🧩 Option 2 — Manual build (using g++)

If you prefer to build manually:

``` bash

g++ src/main.cpp src/MaxHeightCalculator.cpp -o MaxHeightCalculator
./MaxHeightCalculator

```

## 🧮 References

NASA Technical Report:
Propeller Theory & Performance Analysis (NASA NTRS)

U.S. Standard Atmosphere, 1976

QBlade Documentation

## 🪪 License

This project is licensed under the MIT License — see the LICENSE file for details.

## 👨‍💻 Author: GustDustt

Built for educational and experimental purposes — showcasing drone performance modeling using C++, physics and atmosphere simulation.
