// Copyright 2025 UNN-CS
// Nazyrov A.A.

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "TimedDoor.h"

using ::testing::_;

class MockTimerClient : public TimerClient {
 public:
    MOCK_METHOD(void, Timeout, (), (override));
};

class MockDoor : public Door {
 public:
    MOCK_METHOD(void, lock, (), (override));
    MOCK_METHOD(void, unlock, (), (override));
    MOCK_METHOD(bool, isDoorOpened, (), (override));
};

class TimedDoorTest : public ::testing::Test {
 protected:
    void SetUp() override { door = new TimedDoor(3); }
    void TearDown() override { delete door; }
    TimedDoor* door;
};

TEST_F(TimedDoorTest, InitiallyClosed) {
    EXPECT_FALSE(door->isDoorOpened());
}

TEST_F(TimedDoorTest, UnlockOpensDoor) {
    door->unlock();
    EXPECT_TRUE(door->isDoorOpened());
}

TEST_F(TimedDoorTest, LockClosesDoor) {
    door->unlock();
    door->lock();
    EXPECT_FALSE(door->isDoorOpened());
}

TEST_F(TimedDoorTest, GetTimeoutValue) {
    EXPECT_EQ(door->getTimeOut(), 3);
}

TEST_F(TimedDoorTest, ThrowException) {
    EXPECT_THROW(door->throwState(), const char*);
}

TEST(DoorTimerAdapterTest, TimeoutWhenOpen) {
    TimedDoor d(1);
    DoorTimerAdapter ada(d);
    d.unlock();
    EXPECT_THROW(ada.Timeout(), const char*);
}

TEST(DoorTimerAdapterTest, NoTimeoutWhenClosed) {
    TimedDoor d(1);
    DoorTimerAdapter ada(d);
    d.lock();
    EXPECT_NO_THROW(ada.Timeout());
}

TEST(TimerTest, RegisterCallsTimeout) {
    MockTimerClient mock;
    EXPECT_CALL(mock, Timeout()).Times(1);
    Timer t;
    t.tregister(0, &mock);
}

TEST(TimerTest, NullClientSafe) {
    Timer t;
    EXPECT_NO_THROW(t.tregister(0, nullptr));
}
