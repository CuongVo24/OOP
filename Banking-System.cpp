#include <iostream>
#include <vector>
#include <string>
using namespace std;

class Transaction {
public:
    string type; double amount;
    Transaction(string t, double a): type(t), amount(a) {}
};

class Account {
    string number; double balance;
    vector<Transaction> history;
public:
    Account(string no, double bal=0): number(no), balance(bal) {}
    void deposit(double amt){ balance+=amt; history.emplace_back("Deposit", amt); }
    void withdraw(double amt){ if(balance>=amt){ balance-=amt; history.emplace_back("Withdraw", amt);} }
    void displayHistory(){
        cout << "Account " << number << " history:\n";
        for(auto &t : history) cout << "- " << t.type << ": " << t.amount << endl;
        cout << "Current balance: " << balance << endl;
    }
};

int main() {
    Account a("001", 1000);
    a.deposit(500);
    a.withdraw(200);
    a.displayHistory();
}
