# ==========================================================
# MaxHeightCalculator Library Configuration
# ==========================================================


add_library(MaxHeightLib
    MaxHeightCalculator.cpp
    MaxHeightCalculator.hpp
)

target_include_directories(MaxHeightLib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
