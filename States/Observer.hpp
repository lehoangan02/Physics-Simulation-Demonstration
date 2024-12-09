//
// Created by Le Hoang An on 24/10/24.
//

#ifndef PHYSICS_SIMULATION_DEMONSTRATION_OBSERVER_HPP
#define PHYSICS_SIMULATION_DEMONSTRATION_OBSERVER_HPP
#include <list>
class Observer {
public:
    virtual void onNotify() = 0;
};
class Subject {
protected:
    std::list<Observer*> m_Observers;
public:
    virtual void addObserver(Observer* observer);
    virtual void removeObserver(Observer* observer);
    virtual ~Subject() = default;
    void notify();
};


#endif //PHYSICS_SIMULATION_DEMONSTRATION_OBSERVER_HPP
