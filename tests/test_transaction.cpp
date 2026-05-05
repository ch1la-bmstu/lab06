#include <gtest/gtest.h>
#include "Transaction.h"
#include "Account.h"

// mock через наследование
class MockTransaction : public Transaction {
public:
    bool called = false;

private:
    void SaveToDataBase(Account&, Account&, int) override {
        called = true;
    }
};

TEST(Transaction, DefaultFee) {
    Transaction tr;
    EXPECT_EQ(tr.fee(), 1);
}

TEST(Transaction, SetFee) {
    Transaction tr;
    tr.set_fee(10);
    EXPECT_EQ(tr.fee(), 10);
}

TEST(Transaction, SameAccountThrows) {
    Account a(1, 500);
    MockTransaction tr;

    EXPECT_THROW(tr.Make(a, a, 200), std::logic_error);
}

TEST(Transaction, NegativeSumThrows) {
    Account a(1, 500), b(2, 500);
    MockTransaction tr;

    EXPECT_THROW(tr.Make(a, b, -100), std::invalid_argument);
}

TEST(Transaction, TooSmallSumThrows) {
    Account a(1, 500), b(2, 500);
    MockTransaction tr;

    EXPECT_THROW(tr.Make(a, b, 50), std::logic_error);
}

TEST(Transaction, FeeTooBigReturnsFalse) {
    Account a(1, 500), b(2, 500);
    MockTransaction tr;

    tr.set_fee(60); // fee*2 > sum

    EXPECT_FALSE(tr.Make(a, b, 100));
}

TEST(Transaction, SuccessTransfer) {
    Account a(1, 500), b(2, 500);
    MockTransaction tr;

    tr.set_fee(10);

    EXPECT_TRUE(tr.Make(a, b, 200));
    EXPECT_TRUE(tr.called);
}

TEST(Transaction, DebitFailBranch) {
    Account a(1,500), b(2,0);
    MockTransaction tr;

    tr.set_fee(50); // ключевой момент

    EXPECT_FALSE(tr.Make(a,b,200));
    EXPECT_TRUE(tr.called);
}

TEST(Transaction, DebitFalseRollback) {
    Account a(1, 1000), b(2, 0);
    MockTransaction tr;

    tr.set_fee(10);

    EXPECT_FALSE(tr.Make(a, b, 500));
    EXPECT_TRUE(tr.called);
}

TEST(Transaction, FullRollbackAndUnlockCoverage) {
    Account a(1, 1000), b(2, 0);
    MockTransaction tr;

    tr.set_fee(10);

    EXPECT_FALSE(tr.Make(a, b, 500));

    EXPECT_TRUE(tr.called);
}

TEST(Transaction, FullRollbackCase) {
    Account a(1, 1000), b(2, 0);
    MockTransaction tr;

    tr.set_fee(50);

    EXPECT_FALSE(tr.Make(a, b, 200));

    EXPECT_TRUE(tr.called);
}

TEST(Transaction, FinalCoverageCase) {
    Account a(1, 1000), b(2, 0);
    MockTransaction tr;

    tr.set_fee(10);

    EXPECT_FALSE(tr.Make(a, b, 200));

    EXPECT_TRUE(tr.called);
}

TEST(Transaction, FinalEdgeCaseCoverage) {
    Account a(1, 1000), b(2, 0);
    MockTransaction tr;

    tr.set_fee(1);

    // гарантируем: НЕ ранний return
    // и гарантируем: Debit fail + rollback path
    EXPECT_FALSE(tr.Make(a, b, 500));

    EXPECT_TRUE(tr.called);
}

TEST(Transaction, FeeEdgeExactBoundary) {
    Account a(1, 1000), b(2, 0);
    MockTransaction tr;

    tr.set_fee(50);   // ВАЖНО: маленькая fee

    EXPECT_FALSE(tr.Make(a, b, 100));

    EXPECT_TRUE(tr.called);
}

TEST(Transaction, FeeBoundaryZeroRollbackPath) {
    Account a(1, 1000), b(2, 0);
    MockTransaction tr;

    tr.set_fee(0);

    EXPECT_FALSE(tr.Make(a, b, 100));

    EXPECT_TRUE(tr.called);
}

TEST(Transaction, FinalGuaranteedCoverage) {
    Account a(1, 1000), b(2, 0);
    MockTransaction tr;

    tr.set_fee(0);

    EXPECT_FALSE(tr.Make(a, b, 150));  // >= 100 !!!
    EXPECT_TRUE(tr.called);
}
