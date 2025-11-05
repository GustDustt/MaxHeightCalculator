# ==========================================================
# MaxHeightCalculator CMake Configuration
# ==========================================================

cmake_minimum_required(VERSION 3.10)
project(MaxHeightCalculator VERSION 1.0 LANGUAGES CXX)

# ==========================================================
# Compiler configuration
# ==========================================================

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

if (MSVC)
    add_compile_options(/W4)
else()
    add_compile_options(-Wall -Wextra -pedantic)
endif()

# ==========================================================
# Source files
# ==========================================================

add_subdirectory(src)

add_executable(MaxHeightCalculator
    src/main.cpp
)

target_link_libraries(MaxHeightCalculator PRIVATE MaxHeightLib)

# ==========================================================
# Installation (optional)
# ==========================================================

# Installs the final binary to a "bin" folder after "make install"
install(TARGETS MaxHeightCalculator DESTINATION bin)
