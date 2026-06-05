// Copyright 2025 UNN-CS

#include "TimedDoor.h"
#include <thread>
#include <chrono>

// DoorTimerAdapter
DoorTimerAdapter::DoorTimerAdapter(TimedDoor& d) : door(d) {}

void DoorTimerAdapter::Timeout() {
    if (door.isDoorOpened()) {
        door.throwState();
    }
}

// TimedDoor
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
    throw std::runtime_error("Door left open too long!");
}

// Timer
void Timer::sleep(int sec) {
    std::this_thread::sleep_for(std::chrono::seconds(sec));
}

void Timer::tregister(int t, TimerClient* cl) {
    client = cl;
    // ??? ?????? ?? ????, ?????? ???????? Timeout
    if (t == 0 && client) {
        client->Timeout();
    }
}

