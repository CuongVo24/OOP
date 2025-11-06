#include "lib.h"

// CLIENT IMPLEMENTATION
Client::Client() : name(""), clientID("") {}

Client::Client(string name, string clientID)
    : name(name), clientID(clientID) {}

void Client::display() const {
    cout << "Client Name: " << name << endl;
    cout << "Client ID: " << clientID << endl;
}

string Client::getName() const {
    return name;
}

string Client::getID() const {
    return clientID;
}

// ACCOUNT IMPLEMENTATION
Account::Account() : client(), accountNumber(""), balance(0.0) {}

Account::Account(const Client &client, string accountNumber, double balance)
    : client(client), accountNumber(accountNumber), balance(balance) {}

double Account::getBalance() const {
    return balance;
}

void Account::deposit(double amount) {
    if (amount > 0)
        balance += amount;
}

bool Account::withdraw(double amount) {
    if (amount > 0 && amount <= balance) {
        balance -= amount;
        return true;
    }
    return false;
}

void Account::display() const {
    cout << "Account Number: " << accountNumber << endl;
    cout << "Balance: $" << balance << endl;
    cout << "--- Client Info ---" << endl;
    client.display();
}

// SAVINGACCOUNT IMPLEMENTATION
SavingAccount::SavingAccount()
    : Account(), interestRate(0.0) {}

SavingAccount::SavingAccount(const Client &client, string accountNumber, double balance, double interestRate)
    : Account(client, accountNumber, balance), interestRate(interestRate) {}

void SavingAccount::applyInterest() {
    balance += balance * interestRate;
}

void SavingAccount::display() const {
    cout << "==== Saving Account Information ====" << endl;
    Account::display();
    cout << "Interest Rate: " << interestRate * 100 << "%" << endl;
}
