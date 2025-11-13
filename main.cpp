#include "lib.h"

int main() {
    int arr[6] = { 0, 1, 0, 2, 3, 4 };

    cout << "=== Test Array ===\n";
    Array a1(6, arr);
    a1[3] = 4;
    a1.display();

    cout << "\n=== Test HistogramArray ===\n";
    HistogramArray a2(6, arr, 5); // histSize = 5 -> [0, 4]

    a2.computeHist();
    a2.displayHist();

    cout << "\nAfter changing a2[3] = 4:\n";
    a2.setValue(3, 4);
    a2.computeHist();
    a2.displayHist();

    cout << "\nArray content:\n";
    a2.display();

    return 0;
}
