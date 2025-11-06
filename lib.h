#ifndef LIB_H
#define LIB_H

#include <iostream>
#include <string>
using namespace std;

// CLASS CLIENT
class Client {
private:
    string name;
    string clientID;

public:
    Client();
    Client(string name, string clientID);

    void display() const;
    string getName() const;
    string getID() const;
};

// CLASS ACCOUNT
class Account {
protected:
    Client client;
    string accountNumber;
    double balance;

public:
    Account();
    Account(const Client &client, string accountNumber, double balance = 0.0);

    double getBalance() const;
    void deposit(double amount);
    bool withdraw(double amount);
    virtual void display() const;
};

// CLASS SAVINGACCOUNT
class SavingAccount : public Account {
private:
    double interestRate;

public:
    SavingAccount();
    SavingAccount(const Client &client, string accountNumber, double balance, double interestRate);

    void applyInterest();
    void display() const override;
};

#endif
