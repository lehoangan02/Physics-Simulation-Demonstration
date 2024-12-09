//
// Created by Le Hoang An on 26/11/24.
//

#include "Mediator.h"
Mediator::Mediator() {
    m_ActiveState = HomeState::getHomeState();
    m_BackHomeCommand -> addObserver(m_ActiveState);
    m_StateSelector ->setPosition({500, 250});
}
void Mediator::update() {
    SimulationState* NextState = m_ActiveState -> update();
    if (NextState)
    {
        m_BackHomeCommand -> removeObserver(m_ActiveState);
        m_ActiveState -> reset();
        m_ActiveState = NextState;
        m_BackHomeCommand -> addObserver(m_ActiveState);
    }
    m_SoundUtilities -> update();
    if (m_ActiveState -> m_StateNumber == StateNumber::HOME_STATE) {
        m_StateSelector -> m_Active = true;
        unsigned int NextState = m_StateSelector->update();
//        unsigned int NextState = m_StateSelector->update();
        if (NextState == -1)
        {
            NextState = m_ShortcutSelector->update();
        }
        if (NextState != -1) {
            m_BackHomeCommand->removeObserver(m_ActiveState);
            m_ActiveState->reset();
            m_ActiveState = StateFactory::getState(NextState);
            m_BackHomeCommand->addObserver(m_ActiveState);
        }
        m_BackHomeButton->m_Active = false;
    }
    else {
        m_StateSelector -> m_Active = false;
        unsigned int NextState = m_ShortcutSelector->update();
        if (NextState != -1) {
            m_BackHomeCommand->removeObserver(m_ActiveState);
            m_ActiveState->reset();
            m_ActiveState = StateFactory::getState(NextState);
            m_BackHomeCommand->addObserver(m_ActiveState);
        }
        m_BackHomeButton->m_Active = true;
    }
    m_FPSShifter->update();

}
void Mediator::draw() {
    m_ActiveState -> draw();
    m_SoundUtilities -> draw();
    m_BackHomeButton -> update();
    m_BackHomeButton -> draw();
    m_StateSelector -> draw();
}