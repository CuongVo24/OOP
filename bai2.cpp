#include <iostream>
#include <algorithm>
#include <stdexcept>
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
    int _n;

    static Node* CreateNode(const T& value) {
        Node* node = new Node{value, nullptr};
        return node;
    }

public:
    SLList() : _pHead(nullptr), _pTail(nullptr), _n(0) {}

    ~SLList() {
        Clear();
    }

    void Clear() {
        while (_pHead != nullptr) {
            RemoveHead();
        }
    }

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

    void RemoveHead() {
        if (_pHead == nullptr) return;
        Node* temp = _pHead;
        _pHead = _pHead->_pNext;
        if (_pHead == nullptr) _pTail = nullptr;
        delete temp;
        _n--;
    }

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

    class Iterator {
    public:
        using iterator_category = forward_iterator_tag;
        using difference_type = ptrdiff_t;
        using value_type = T;
        using pointer = T*;
        using reference = T&;

    private:
        Node* current_node;

    public:
        Iterator(Node* node = nullptr) : current_node(node) {}

        reference operator*() const {
            if (!current_node) throw out_of_range("Dereferencing end iterator");
            return current_node->_info;
        }

        Iterator& operator++() {
            if (current_node) current_node = current_node->_pNext;
            return *this;
        }

        Iterator operator++(int) {
            Iterator temp = *this;
            ++(*this);
            return temp;
        }

        bool operator!=(const Iterator& other) const {
            return current_node != other.current_node;
        }
        
        bool operator==(const Iterator& other) const {
            return current_node == other.current_node;
        }
    };

    Iterator begin() { return Iterator(_pHead); }
    Iterator end() { return Iterator(nullptr); }
};

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
    for_each(l.begin(), l.end(), fnAction2);
    for_each(l.begin(), l.end(), fnAction);
    cout << endl;

    cout << "Replace_if (chan -> 100): ";
    replace_if(l.begin(), l.end(), fnPredict, 100);
    for_each(l.begin(), l.end(), fnAction);
    cout << endl;

    cout << "Fill (123): ";
    fill(l.begin(), l.end(), 123);
    for_each(l.begin(), l.end(), fnAction);
    cout << endl;

    system("pause");
    return 0;
}
