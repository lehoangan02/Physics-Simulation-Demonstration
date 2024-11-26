//
// Created by Le Hoang An on 26/11/24.
//

#ifndef PHYSICS_SIMULATION_DEMONSTRATION_MEDIATOR_H
#define PHYSICS_SIMULATION_DEMONSTRATION_MEDIATOR_H

#endif //PHYSICS_SIMULATION_DEMONSTRATION_MEDIATOR_H
#include <vector>
#include "../States/SimulationState.hpp"
#include "Utilities.hpp"
class Mediator
{
public:
    Mediator();
    void update();
    void draw();
    ~Mediator();
private:
    BackHomeButton* m_BackHomeButton = BackHomeButton::getBackHomeButton();
    Command* m_BackHomeCommand = BackHome::getBackHome();
    SimulationState* m_ActiveState;
    StateSelector* m_StateSelector = StateSelector::getStateSelector();
    SoundUtilities* m_SoundUtilities = SoundUtilities::getUtilities();
    FPSShifter* m_FPSShifter = FPSShifter::getFPSShifter();
    ShortcutSelector* m_ShortcutSelector = ShortcutSelector::getShortcutSelector();
};