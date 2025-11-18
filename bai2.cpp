#include <iostream>
#include <algorithm> // Để dùng for_each, replace_if, fill [cite: 366]
#include <stdexcept> // Để dùng out_of_range

using namespace std;

template <class T>
class SLList {
private:
    struct Node {
        T _info;
        Node* _pNext;
    };

    Node* _pHead;
    Node* _pTail;
    int _n; // Số lượng phần tử

    // Helper tạo node
    static Node* CreateNode(const T& value) {
        Node* node = new Node{value, nullptr}; // [cite: 198]
        return node;
    }

public:
    SLList() : _pHead(nullptr), _pTail(nullptr), _n(0) {} // [cite: 236]

    // 1. Destructor: Xóa sạch bộ nhớ [cite: 238]
    ~SLList() {
        Clear();
    }

    void Clear() {
        while (_pHead != nullptr) {
            RemoveHead();
        }
    }

    // 2. AddHead: Thêm vào đầu [cite: 279]
    void AddHead(const T& value) {
        Node* node = CreateNode(value);
        if (node == nullptr) return;
        
        if (_pHead == nullptr) {
            _pHead = _pTail = node;
        } else {
            node->_pNext = _pHead;
            _pHead = node;
        }
        _n++;
    }

    // AddTail (Đã có trong đề, viết lại cho đủ) [cite: 261]
    void AddTail(const T& value) {
        Node* node = CreateNode(value);
        if (node == nullptr) return;
        if (_pHead == nullptr) {
            _pHead = _pTail = node;
        } else {
            _pTail->_pNext = node;
            _pTail = node;
        }
        _n++;
    }

    // 3. RemoveHead: Xóa đầu [cite: 283]
    void RemoveHead() {
        if (_pHead == nullptr) return;
        Node* temp = _pHead;
        _pHead = _pHead->_pNext;
        if (_pHead == nullptr) _pTail = nullptr;
        delete temp;
        _n--;
    }

    // 4. RemoveTail: Xóa đuôi (Cần duyệt vì là DSLK đơn) [cite: 293]
    void RemoveTail() {
        if (_pHead == nullptr) return;
        if (_pHead == _pTail) {
            RemoveHead();
            return;
        }

        Node* temp = _pHead;
        while (temp->_pNext != _pTail) {
            temp = temp->_pNext;
        }
        delete _pTail;
        _pTail = temp;
        _pTail->_pNext = nullptr;
        _n--;
    }

    // --- ITERATOR CLASS DEFINITION [cite: 298] ---
    class Iterator {
    public:
        // Iterator Traits cho STL
        using iterator_category = forward_iterator_tag; // [cite: 303]
        using difference_type = ptrdiff_t;
        using value_type = T;
        using pointer = T*;
        using reference = T&;

    private:
        Node* current_node;

    public:
        Iterator(Node* node = nullptr) : current_node(node) {}

        // Toán tử * (dereference) [cite: 313]
        reference operator*() const {
            if (!current_node) throw out_of_range("Dereferencing end iterator");
            return current_node->_info;
        }

        // Toán tử ++ (prefix) [cite: 337]
        Iterator& operator++() {
            if (current_node) current_node = current_node->_pNext;
            return *this;
        }

        // Toán tử ++ (postfix)
        Iterator operator++(int) {
            Iterator temp = *this;
            ++(*this);
            return temp;
        }

        bool operator!=(const Iterator& other) const { // [cite: 364]
            return current_node != other.current_node;
        }
        
        bool operator==(const Iterator& other) const {
            return current_node == other.current_node;
        }
    };

    Iterator begin() { return Iterator(_pHead); } // [cite: 216]
    Iterator end() { return Iterator(nullptr); }   // [cite: 218]==
};

// Các hàm hỗ trợ test STL [cite: 367-374]
void fnAction(int val) { cout << val << ", "; }
void fnAction2(int &val) { val *= 2; }
bool fnPredict(int val) { 
  if(val == 2 || val == 0) return false;
  return val % 2 == 0;
}

int main() {
    SLList<int> l;
    l.AddTail(1); l.AddTail(1); l.AddTail(0); l.AddTail(4); l.AddTail(2);  l.AddTail(5);// [cite: 395-401]

    cout << "Duyet iterator: ";
    for (SLList<int>::Iterator it = l.begin(); it != l.end(); ++it) {
        cout << *it << " ";
    }
    cout << endl;

    cout << "For_each (nhan doi): ";
    for_each(l.begin(), l.end(), fnAction2); // [cite: 408]
    for_each(l.begin(), l.end(), fnAction);
    cout << endl;

    cout << "Replace_if (chan -> 100): ";
    // Lưu ý: replace_if yêu cầu Forward Iterator có khả năng ghi (reference)
    replace_if(l.begin(), l.end(), fnPredict, 100); // [cite: 414]
    for_each(l.begin(), l.end(), fnAction);
    cout << endl;

    cout << "Fill (123): ";
    fill(l.begin(), l.end(), 123); // [cite: 418]
    for_each(l.begin(), l.end(), fnAction);
    cout << endl;

    system("pause");
    return 0;
}
