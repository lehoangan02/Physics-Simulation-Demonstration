cmake_minimum_required(VERSION 3.27)
project(Physics_Simulation_Demonstration)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR})

add_executable(Physics_Simulation_Demonstration main.cpp
        Physics/RoundBall.hpp
        Physics/RoundBall.cpp
        Physics/Engine.cpp
        Physics/Engine.hpp
        Physics/PlatformTriangle.hpp
        States/SimulationState.hpp
        States/SimulationState.cpp
        Physics/PlatformTriangle.cpp
        Math/Geometry.cpp
        Math/Geometry.hpp
        Math/EquationsSolver.cpp
        Math/EquationsSolver.hpp
        Commands/Command.cpp
        Commands/Command.hpp
        Buttons/Buttons.cpp
        Buttons/Buttons.hpp
        Utilites/Utilities.hpp
        Utilites/Utilities.cpp
        Utilites/BackgroundMusic.cpp
        Utilites/BackgroundMusic.hpp
        States/Observer.cpp
        States/Observer.hpp
        Physics/Chains.cpp
        Physics/Chains.hpp
        Physics/Spring.cpp
        Physics/Spring.hpp
        Physics/PlatformRectangle.cpp
        Physics/PlatformRectangle.h
        Utilites/Mediator.cpp
        Utilites/Mediator.h
        "Machine Learning/KMeansCalculator.cpp"
        "Machine Learning/KMeansCalculator.h"
        Physics/Collider.cpp
        Physics/Collider.h
        Physics/SATPlatform.cpp
        Physics/SATPlatform.hpp
        Physics/GeneralPhysics.cpp
        Physics/GeneralPhysics.hpp
        Animation/Animation.cpp
        Animation/Animation.hpp)
include_directories(/opt/homebrew/Cellar/raylib/5.0/include)

find_package(raylib 4.5 REQUIRED)
target_link_libraries(Physics_Simulation_Demonstration raylib)


