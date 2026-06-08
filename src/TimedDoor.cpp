// Copyright 2025 UNN-CS
// Nazyrov A.A.

#include "TimedDoor.h"
#include <thread>
#include <chrono>
#include <stdexcept>

DoorTimerAdapter::DoorTimerAdapter(TimedDoor& d) : door(d) {}

void DoorTimerAdapter::Timeout() {
    if (door.isDoorOpened()) {
        door.throwState();
    }
}

TimedDoor::TimedDoor(int t) : iTimeout(t), isOpened(false) {
    adapter = new DoorTimerAdapter(*this);
}

TimedDoor::~TimedDoor() {
    delete adapter;
}

bool TimedDoor::isDoorOpened() {
    return isOpened;
}

void TimedDoor::unlock() {
    isOpened = true;
}

void TimedDoor::lock() {
    isOpened = false;
}

int TimedDoor::getTimeOut() const {
    return iTimeout;
}

void TimedDoor::throwState() {
    throw std::runtime_error("Door timeout!");
}

void Timer::sleep(int sec) {
    std::this_thread::sleep_for(std::chrono::seconds(sec));
}

void Timer::tregister(int sec, TimerClient* cl) {
    client = cl;
    if (sec == 0 && client) {
        client->Timeout();
    }
}
