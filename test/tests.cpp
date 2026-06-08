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

TEST_F(TimedDoorTest, DoubleUnlock) {
    TimedDoor d(1);
    d.unlock();
    d.unlock();
    EXPECT_TRUE(d.isDoorOpened());
}

TEST_F(TimedDoorTest, LockWithoutUnlock) {
    TimedDoor d(1);
    d.lock();
    EXPECT_FALSE(d.isDoorOpened());
}

TEST_F(TimedDoorTest, DifferentTimeout) {
    TimedDoor d1(1);
    TimedDoor d2(5);
    EXPECT_EQ(d1.getTimeOut(), 1);
    EXPECT_EQ(d2.getTimeOut(), 5);
}

class DoorTimerAdapterTest : public ::testing::Test {
 protected:
    void SetUp() override { door = new TimedDoor(1); }
    void TearDown() override { delete door; }
    TimedDoor* door;
};

TEST_F(DoorTimerAdapterTest, TimeoutWhenOpen) {
    DoorTimerAdapter ada(*door);
    door->unlock();
    EXPECT_THROW(ada.Timeout(), const char*);
}

TEST_F(DoorTimerAdapterTest, NoTimeoutWhenClosed) {
    DoorTimerAdapter ada(*door);
    door->lock();
    EXPECT_NO_THROW(ada.Timeout());
}

TEST_F(DoorTimerAdapterTest, MultipleAdapters) {
    DoorTimerAdapter ada1(*door);
    DoorTimerAdapter ada2(*door);
    door->unlock();
    EXPECT_THROW(ada1.Timeout(), const char*);
    EXPECT_THROW(ada2.Timeout(), const char*);
}

class TimerTest : public ::testing::Test {
 protected:
    void SetUp() override { timer = new Timer(); }
    void TearDown() override { delete timer; }
    Timer* timer;
};

TEST_F(TimerTest, RegisterCallsTimeout) {
    MockTimerClient mock;
    EXPECT_CALL(mock, Timeout()).Times(1);
    timer->tregister(0, &mock);
}

TEST_F(TimerTest, NullClientSafe) {
    EXPECT_NO_THROW(timer->tregister(0, nullptr));
}

class IntegrationTest : public ::testing::Test {
 protected:
    void SetUp() override { door = new TimedDoor(1); }
    void TearDown() override { delete door; }
    TimedDoor* door;
};

TEST_F(IntegrationTest, UnlockLockUnlock) {
    door->unlock();
    door->lock();
    door->unlock();
    EXPECT_TRUE(door->isDoorOpened());
}

TEST_F(IntegrationTest, TwoDoorsIndependent) {
    TimedDoor d1(1);
    TimedDoor d2(2);
    d1.unlock();
    d2.unlock();
    EXPECT_TRUE(d1.isDoorOpened());
    EXPECT_TRUE(d2.isDoorOpened());
    EXPECT_EQ(d1.getTimeOut(), 1);
    EXPECT_EQ(d2.getTimeOut(), 2);
}
