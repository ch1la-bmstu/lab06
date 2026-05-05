#include <gtest/gtest.h>
#include "Account.h"

TEST(Account, Constructor) {
    Account acc(1, 100);
    EXPECT_EQ(acc.id(), 1);
    EXPECT_EQ(acc.GetBalance(), 100);
}

TEST(Account, ChangeBalanceThrowsIfNotLocked) {
    Account acc(1, 100);
    EXPECT_THROW(acc.ChangeBalance(50), std::runtime_error);
}

TEST(Account, ChangeBalanceWorksIfLocked) {
    Account acc(1, 100);

    acc.Lock();
    acc.ChangeBalance(50);
    acc.Unlock();

    EXPECT_EQ(acc.GetBalance(), 150);
}

TEST(Account, DoubleLockThrows) {
    Account acc(1, 100);

    acc.Lock();
    EXPECT_THROW(acc.Lock(), std::runtime_error);
}

TEST(Account, UnlockWorks) {
    Account acc(1, 100);

    acc.Lock();
    acc.Unlock();

    // после unlock снова можно lock
    EXPECT_NO_THROW(acc.Lock());
}

TEST(Account, ChangeBalanceThrowsDirectly) {
    Account a(1, 100);

    EXPECT_THROW(a.ChangeBalance(50), std::runtime_error);
}

TEST(Account, DoubleLockThrowsCoverage) {
    Account a(1, 100);

    a.Lock();
    EXPECT_THROW(a.Lock(), std::runtime_error);
}

TEST(Account, ChangeBalanceUnlockedExceptionPath) {
    Account a(1, 100);

    EXPECT_THROW(a.ChangeBalance(50), std::runtime_error);
}
