#include <iostream>
using namespace std;

// Comparison functions
bool ascending(int a, int b) {
    return a > b; // swap if a > b
}

bool descending(int a, int b) {
    return a < b; // swap if a < b
}

// Sort function using a function pointer for comparison
void sortArray(int arr[], int n, bool (*compare)(int, int)) {
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (compare(arr[j], arr[j + 1])) {
                // Swap
                int temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
            }
        }
    }
}

// Utility function to print the array
void printArray(int arr[], int n) {
    for (int i = 0; i < n; i++)
        cout << arr[i] << " ";
    cout << endl;
}

int main() {
    int arr[] = {5, 1, 9, 3, 7};
    int n = sizeof(arr) / sizeof(arr[0]);
    int choice;

    cout << "Choose sorting order:\n";
    cout << "1. Ascending\n";
    cout << "2. Descending\n";
    cout << "Enter choice: ";
    cin >> choice;

    if (choice == 1)
        sortArray(arr, n, ascending);
    else if (choice == 2)
        sortArray(arr, n, descending);
    else {
        cout << "Invalid choice.\n";
        return 0;
    }

    cout << "Sorted array: ";
    printArray(arr, n);

    return 0;
}
