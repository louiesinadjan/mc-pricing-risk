cmake_minimum_required(VERSION 3.15)
project(monte-carlo-pricing CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

include(CompilerOptions)
include(Sanitizers)
include(Warnings)

add_library(mc-lib
    src/simulation/simulation.cpp
    src/model/GBM.cpp
    src/randomness/stdnormalrng.cpp
    src/payoff/european.cpp
)

target_include_directories(mc-lib PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

add_executable(mc-pricing src/main.cpp)
target_link_libraries(mc-pricing PRIVATE mc-lib)