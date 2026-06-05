// Copyright 2025 UNN-CS

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "TimedDoor.h"

using ::testing::_;

// Mock-класс для TimerClient
class MockTimerClient : public TimerClient {
  public:
    MOCK_METHOD(void, Timeout, (), (override));
};

// Mock-класс для Door
class MockDoor : public Door {
  public:
    MOCK_METHOD(void, lock, (), (override));
    MOCK_METHOD(void, unlock, (), (override));
    MOCK_METHOD(bool, isDoorOpened, (), (override));
};

// Тест-фикстура для TimedDoor
class TimedDoorTest : public ::testing::Test {
  protected:
    void SetUp() override {
        door = new TimedDoor(2);
    }
    void TearDown() override {
        delete door;
    }
    TimedDoor* door;
};

// ========== Тесты TimedDoor (6 тестов) ==========

TEST_F(TimedDoorTest, InitiallyClosed) {
    EXPECT_FALSE(door->isDoorOpened());
}

TEST_F(TimedDoorTest, UnlockOpens) {
    door->unlock();
    EXPECT_TRUE(door->isDoorOpened());
}

TEST_F(TimedDoorTest, LockCloses) {
    door->unlock();
    door->lock();
    EXPECT_FALSE(door->isDoorOpened());
}

TEST_F(TimedDoorTest, GetTimeOutWorks) {
    EXPECT_EQ(door->getTimeOut(), 2);
}

TEST_F(TimedDoorTest, ThrowStateThrows) {
    EXPECT_THROW(door->throwState(), const char*);
}

TEST_F(TimedDoorTest, DoubleUnlock) {
    door->unlock();
    door->unlock();
    EXPECT_TRUE(door->isDoorOpened());
}

TEST_F(TimedDoorTest, DifferentTimeout) {
    TimedDoor d2(5);
    EXPECT_EQ(d2.getTimeOut(), 5);
}

// ========== Тесты DoorTimerAdapter (4 теста) ==========

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

TEST(DoorTimerAdapterTest, TimeoutAfterLock) {
    TimedDoor d(1);
    DoorTimerAdapter ada(d);
    d.unlock();
    d.lock();
    EXPECT_NO_THROW(ada.Timeout());
}

TEST(DoorTimerAdapterTest, MultipleAdapters) {
    TimedDoor d(1);
    DoorTimerAdapter ada1(d);
    DoorTimerAdapter ada2(d);
    d.unlock();
    EXPECT_THROW(ada1.Timeout(), const char*);
    EXPECT_THROW(ada2.Timeout(), const char*);
}

// ========== Тесты Timer (4 теста) ==========

TEST(TimerTest, RegisterCallsTimeout) {
    MockTimerClient mock;
    EXPECT_CALL(mock, Timeout()).Times(1);
    Timer t;
    t.tregister(0, &mock);
}

TEST(TimerTest, NullClientNoCrash) {
    Timer t;
    EXPECT_NO_THROW(t.tregister(0, nullptr));
}

TEST(TimerTest, MultipleRegisters) {
    MockTimerClient mock1, mock2;
    EXPECT_CALL(mock1, Timeout()).Times(1);
    EXPECT_CALL(mock2, Timeout()).Times(1);
    Timer t;
    t.tregister(0, &mock1);
    t.tregister(0, &mock2);
}

TEST(TimerTest, SameClientTwice) {
    MockTimerClient mock;
    EXPECT_CALL(mock, Timeout()).Times(2);
    Timer t;
    t.tregister(0, &mock);
    t.tregister(0, &mock);
}

// ========== Интеграционные тесты (3 теста) ==========

TEST(IntegrationTest, UnlockLockUnlock) {
    TimedDoor d(1);
    d.unlock();
    d.lock();
    d.unlock();
    EXPECT_TRUE(d.isDoorOpened());
}

TEST(IntegrationTest, TwoDoorsIndependent) {
    TimedDoor d1(1);
    TimedDoor d2(2);
    d1.unlock();
    d2.unlock();
    EXPECT_TRUE(d1.isDoorOpened());
    EXPECT_TRUE(d2.isDoorOpened());
    EXPECT_EQ(d1.getTimeOut(), 1);
    EXPECT_EQ(d2.getTimeOut(), 2);
}

TEST(IntegrationTest, AdapterAfterDestruction) {
    TimedDoor* d = new TimedDoor(1);
    DoorTimerAdapter ada(*d);
    d->unlock();
    EXPECT_THROW(ada.Timeout(), const char*);
    delete d;
}
