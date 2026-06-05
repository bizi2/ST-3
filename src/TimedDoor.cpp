// Copyright 2025 UNN-CS

#include "TimedDoor.h"

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
    throw "Door left open!";
}

// Timer (?????? ?????????? ??? ??????)
Timer::Timer() : client(nullptr) {}

void Timer::sleep(int sec) {
    // ?????? ????????
}

void Timer::tregister(int t, TimerClient* cl) {
    client = cl;
    // ?? ???????? Timeout ?????????????
}

