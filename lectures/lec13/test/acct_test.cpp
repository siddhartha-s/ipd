#include "Bank_account.h"
#include <UnitTest++/UnitTest++.h>

TEST(NewAccount)
{
    Bank_account acct(1, "Robby");
    CHECK_EQUAL(1, acct.id());
    CHECK_EQUAL("Robby", acct.owner());
    CHECK_EQUAL(0, acct.balance());
}

TEST(Deposit)
{
    Bank_account acct(2, "Jesse");
    acct.deposit(50);
    CHECK_EQUAL(50, acct.balance());
    acct.deposit(25);
    CHECK_EQUAL(75, acct.balance());
}

TEST(Withdrawal_success)
{
    Bank_account acct(2, "Jesse");
    acct.deposit(50);
    CHECK_EQUAL(50, acct.balance());
    CHECK_EQUAL(true, acct.withdraw(20));
    CHECK_EQUAL(30, acct.balance());
}

TEST(Withdrawal_failure)
{
    Bank_account acct(2, "Jesse");
    acct.deposit(50);
    CHECK_EQUAL(50, acct.balance());
    CHECK_EQUAL(false, acct.withdraw(70));
    CHECK_EQUAL(50, acct.balance());
}

TEST(Steal)
{
    Bank_account acct(3, "Robby");
    acct.deposit(10000);
    CHECK_EQUAL("Robby", acct.owner());
    acct.change_owner("Jesse");
    CHECK_EQUAL("Jesse", acct.owner());
}
