#include <iostream>
#include <string>
using namespace std;

// ---------------------- CLASS CLIENT ----------------------
class Client {
private:
    string name;
    string id;

public:
    // Constructor
    Client(string n, string i) : name(n), id(i) {}

    // Getters
    string getName() const { return name; }
    string getId() const { return id; }

    // Setters (có kiểm tra)
    void setName(const string& n) {
        if (!n.empty()) name = n;
        else cout << "Invalid name.\n";
    }

    void setId(const string& i) {
        if (!i.empty()) id = i;
        else cout << "Invalid ID.\n";
    }

    void display() const {
        cout << "Client: " << name << " (" << id << ")\n";
    }
};

// ---------------------- CLASS ACCOUNT ----------------------
class Account { // Accouny HAS-A Client
private:
    Client client; // quan hệ mạnh, sở hữu thông tin client
    string accNo;
    double balance;

protected: // Cho phép lớp con truy cập qua getter
    // Các phương thức protected để lớp con truy cập an toàn
    double& accessBalance() { return balance; }
    const Client& getClientRef() const { return client; }

public:
    // Constructor
    Account(const Client& c, const string& no, double bal = 0)
        : client(c), accNo(no), balance(bal) {}

    // Virtual destructor (quan trọng nếu dùng kế thừa)
    virtual ~Account() {}

    // Các thao tác cơ bản
    virtual void deposit(double amt) {
        if (amt > 0) balance += amt;
        else cout << "Deposit amount must be positive.\n";
    }

    virtual void withdraw(double amt) {
        if (amt <= 0) {
            cout << "Withdraw amount must be positive.\n";
        } else if (amt > balance) {
            cout << "Insufficient balance.\n";
        } else {
            balance -= amt;
        }
    }

    // Getters
    double getBalance() const { return balance; }
    string getAccountNumber() const { return accNo; }
    Client getClient() const { return client; }

    // Display
    virtual void display() const {
        cout << "[Account Info]\n";
        client.display();
        cout << "Account No: " << accNo << "\n"
             << "Balance: " << balance << "\n";
    }
};

// ---------------------- CLASS SAVING ACCOUNT ----------------------
class SavingAccount : public Account {// SavingAccount IS-A Account. Mở rộng, sửa đổi hành vi của Account
private:
    double interestRate; // phần trăm lãi suất, ví dụ 0.05 = 5%

public:
    // Constructor
    SavingAccount(const Client& c, const string& no, double bal, double rate)
        : Account(c, no, bal), interestRate(rate) {}

void applyInterest() {
        double& bal = accessBalance(); // ✅ truy cập qua hàm protected
        bal += bal * interestRate;
    }

    // Override display
    void display() const override { //ghi đè thêm chức năng lãi xuất
        cout << "[Saving Account]\n";
        Account::display();
        cout << "Interest rate: " << interestRate * 100 << "%\n";
    }
    ~SavingAccount() {
        cout << "SavingAccount destructor called\n";
    }
};

// ---------------------- MAIN ----------------------
int main() {
    Client c("Cuong Vo", "C001");
    SavingAccount s(c, "A001", 1000, 0.05);

    cout << "=== Before Interest ===\n";
    s.display();

    cout << "\nApplying interest...\n";
    s.applyInterest();

    cout << "\n=== After Interest ===\n";
    s.display();

    cout << "\nDeposit 500...\n";
    s.deposit(500);
    s.display();

    cout << "\nWithdraw 200...\n";
    s.withdraw(200);
    s.display();

    return 0;
}
