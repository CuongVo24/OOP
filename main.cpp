#include "lib.h"

int main() {
    Address addr1("17 Vo Thi Sau", "Phuong AyunPa", "Gia Lai");
    Student s1("Cuong Vo", "24120029", 3.8, addr1);

    Address addr2("Bui bua do", "Binh Nguyen Vo Tan", "Lien Quan mobile");
    ExchangeStudent ex1("Kahlii", "00001", 3.6, addr2, "Vietnam", 12);

    cout << "=== Student Information ===" << endl;
    s1.display();
    cout << endl;

    cout << "=== Exchange Student Information ===" << endl;
    ex1.display();
    cout << endl;

    // Update duration
    ex1.updateDuration(18);
    cout << "After updating duration:" << endl;
    ex1.display();

    return 0;
}
