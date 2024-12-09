//
// Created by Le Hoang An on 23/10/24.
//
#include "Command.hpp"

Command *SoundOff::getSoundOff() {
    static SoundOff soundOff;
    return &soundOff;
}
void SoundOff::execute() {
    BackgroundMusic::getBackgroundMusic() -> turnOff();
}
Command *SoundOn::getSoundOn() {
    static SoundOn soundOn;
    return &soundOn;
}
void SoundOn::execute() {
    BackgroundMusic::getBackgroundMusic() -> turnOn();
}

Command *BackHome::getBackHome() {
    static BackHome backHome;
    return &backHome;
}
void BackHome::execute() {
    notify();
}
void BackHome::addObserver(Observer *observer) {
    m_Observers.push_back(observer);
}
void BackHome::removeObserver(Observer *observer) {
    m_Observers.remove(observer);
}