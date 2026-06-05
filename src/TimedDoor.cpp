// Copyright 2025 UNN-CS

#include "TimedDoor.h"
#include <thread>
#include <chrono>
#include <stdexcept>

// DoorTimerAdapter implementation
DoorTimerAdapter::DoorTimerAdapter(TimedDoor& d) : door(d) {}

void DoorTimerAdapter::Timeout() {
  if (door.isDoorOpened()) {
    door.throwState();
  }
}

// TimedDoor implementation
TimedDoor::TimedDoor(int timeout) : iTimeout(timeout), isOpened(false) {
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
  throw std::runtime_error("Door was left open too long!");
}

// Timer implementation
Timer::Timer() : client(nullptr) {}

void Timer::sleep(int sec) {
  std::this_thread::sleep_for(std::chrono::seconds(sec));
}

void Timer::tregister(int timeout, TimerClient* cl) {
  client = cl;
  sleep(timeout);
  if (client) {
    client->Timeout();
  }
}
