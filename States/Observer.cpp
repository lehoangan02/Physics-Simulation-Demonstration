//
// Created by Le Hoang An on 24/10/24.
//

#include "Observer.hpp"
void Subject::addObserver(Observer *observer) {
    m_Observers.push_back(observer);
}
void Subject::removeObserver(Observer *observer) {
    m_Observers.remove(observer);
}
void Subject::notify() {
    for (auto observer : m_Observers) {
        observer->onNotify();
    }
}