#include <iostream>
using namespace std;

// Template function to sort an array of any type
template <typename T>
void sortArray(T arr[], int n) {
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (arr[j] > arr[j + 1]) {
                // Swap
                T temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
            }
        }
    }
}

// Template function to print an array
template <typename T>
void printArray(T arr[], int n) {
    for (int i = 0; i < n; i++) {
        cout << arr[i] << " ";
    }
    cout << endl;
}

int main() {
    int intArr[] = {5, 2, 9, 1, 5, 6};
    float floatArr[] = {3.2, 1.5, 4.8, 2.9};
    char charArr[] = {'z', 'b', 'a', 'k'};

    int n1 = sizeof(intArr) / sizeof(intArr[0]);
    int n2 = sizeof(floatArr) / sizeof(floatArr[0]);
    int n3 = sizeof(charArr) / sizeof(charArr[0]);

    sortArray(intArr, n1);
    sortArray(floatArr, n2);
    sortArray(charArr, n3);

    cout << "Sorted int array: ";
    printArray(intArr, n1);

    cout << "Sorted float array: ";
    printArray(floatArr, n2);

    cout << "Sorted char array: ";
    printArray(charArr, n3);

    return 0;
}
