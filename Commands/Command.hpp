//
// Created by Le Hoang An on 23/10/24.
//

#ifndef PHYSICS_SIMULATION_DEMONSTRATION_COMMAND_HPP
#define PHYSICS_SIMULATION_DEMONSTRATION_COMMAND_HPP
#include "../Utilites/BackgroundMusic.hpp"
#include "../States/SimulationState.hpp"
#include "../States/Observer.hpp"
class Command: public Subject {
public:
    virtual void execute() = 0;
    virtual ~Command() = default;
};
class BackHome : public Command{
private:
//    SimulationState* m_SimulationState = HomeState::getHomeState();
public:
    void execute() override;
    static Command* getBackHome();
    void addObserver(Observer* observer) override;
    void removeObserver(Observer* observer) override;
private:
    ~BackHome() = default;
    BackHome() = default;
};
class SoundOn : public Command {
public:
    void execute() override;
    static Command* getSoundOn();
private:
    ~SoundOn() = default;
    SoundOn() = default;
};
class SoundOff : public Command {
public:
    void execute() override;
    static Command* getSoundOff();
private:
    ~SoundOff() = default;
    SoundOff() = default;
};
#endif //PHYSICS_SIMULATION_DEMONSTRATION_COMMAND_HPP
