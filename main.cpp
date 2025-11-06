#include "lib.h"

int main() {
    Client client1("Vo Van Cuong", "C001");
    Account acc1(client1, "ACC123", 1000.0);
    SavingAccount sav1(client1, "SAV456", 2000.0, 0.05);

    cout << "\n--- Normal Account Info ---" << endl;
    acc1.display();

    cout << "\nDepositing 500..." << endl;
    acc1.deposit(500);
    acc1.display();

    cout << "\nWithdrawing 300..." << endl;
    acc1.withdraw(300);
    acc1.display();

    cout << "\n--- Saving Account Info ---" << endl;
    sav1.display();

    cout << "\nApplying Interest..." << endl;
    sav1.applyInterest();
    sav1.display();

    return 0;
}
