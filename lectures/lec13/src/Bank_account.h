#pragma once

#include <string>

class Bank_account
{
public:
    // Creates a new bank account with the given account number and owner name.
    Bank_account(unsigned long, std::string&);

    // Returns the account number.
    unsigned long id() const;
    // Returns the name of the account owner.
    std::string& owner() const;
    // Returns the balance of the account.
    unsigned int balance() const;

    // Assigns the account a different owner.
    void change_owner(std::string&);
    // Deposits the given quantity in the account.
    void deposit(unsigned int);
    // Attempts to withdraw the given quantity from the account; returns
    // `true` if the withdrawal succeeds and `false` otherwise.
    bool withdrawal(unsigned int);

private:
    unsigned long id_;
    std::string owner_;
    unsigned int balance_;
};