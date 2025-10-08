#include <iostream>
using namespace std;

// Comparison functions (generic)
template <typename T>
bool ascending(T a, T b) {
    return a > b; // Swap if a > b
}

template <typename T>
bool descending(T a, T b) {
    return a < b; // Swap if a < b
}

// Generic sort function using function pointer
template <typename T>
void sortArray(T arr[], int n, bool (*compare)(T, T)) {
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (compare(arr[j], arr[j + 1])) {
                T temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
            }
        }
    }
}

// Print function template
template <typename T>
void printArray(T arr[], int n) {
    for (int i = 0; i < n; i++)
        cout << arr[i] << " ";
    cout << endl;
}

int main() {
    int intArr[] = {5, 2, 9, 1, 6};
    float floatArr[] = {3.5, 1.2, 7.8, 0.4};
    char charArr[] = {'z', 'b', 'a', 'm'};

    int choice;
    cout << "Choose sorting order:\n";
    cout << "1. Ascending\n";
    cout << "2. Descending\n";
    cout << "Enter choice: ";
    cin >> choice;

    if (choice == 1) {
        cout << "\nSorted int array: ";
        sortArray(intArr, 5, ascending);
        printArray(intArr, 5);

        cout << "Sorted float array: ";
        sortArray(floatArr, 4, ascending);
        printArray(floatArr, 4);

        cout << "Sorted char array: ";
        sortArray(charArr, 4, ascending);
        printArray(charArr, 4);
    }
    else if (choice == 2) {
        cout << "\nSorted int array: ";
        sortArray(intArr, 5, descending);
        printArray(intArr, 5);

        cout << "Sorted float array: ";
        sortArray(floatArr, 4, descending);
        printArray(floatArr, 4);

        cout << "Sorted char array: ";
        sortArray(charArr, 4, descending);
        printArray(charArr, 4);
    }
    else {
        cout << "Invalid choice.\n";
    }

    return 0;
}
