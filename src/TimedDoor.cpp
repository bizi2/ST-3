// Copyright 2025 UNN-CS

#include "TimedDoor.h"
#include <thread>
#include <chrono>

DoorTimerAdapter::DoorTimerAdapter(TimedDoor& d) : door(d) {}

void DoorTimerAdapter::Timeout() {
    if (door.isDoorOpened()) {
        door.throwState();
    }
}

TimedDoor::TimedDoor(int t) : iTimeout(t), isOpened(false) {
    adapter = new DoorTimerAdapter(*this);
}

bool TimedDoor::isDoorOpened() {
    return isOpened;
}

void TimedDoor::unlock() {
    isOpened = true;
    Timer tm;
    tm.tregister(iTimeout, adapter);
}

void TimedDoor::lock() {
    isOpened = false;
}

int TimedDoor::getTimeOut() const {
    return iTimeout;
}

void TimedDoor::throwState() {
    throw "Door left open!";
}

void Timer::sleep(int sec) {
    std::this_thread::sleep_for(std::chrono::seconds(sec));
}

void Timer::tregister(int t, TimerClient* cl) {
    client = cl;
    sleep(t);
    if (client) {
        client->Timeout();
    }
}

