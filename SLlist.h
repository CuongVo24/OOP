#include <iostream>
#include <algorithm>

using namespace std;

template<class T>
class SLList
{
private:
    struct Node
    {
        T _info;
        Node* _pNext;
    };
    Node* _pHead, * _pTail;
    int _n;
    static Node* CreateNode(const T& value)
    {
        Node* node = new Node{ value, nullptr };
        return node;
    }

public:
    class Iterator;
    SLList();
    ~SLList();
    void AddHead(const T&);
    void AddTail(const T&);
    void RemoveHead();
    void RemoveTail();
    void clear();

    Iterator begin() {
        return Iterator(_pHead);
    }
    Iterator end() {
        return Iterator(nullptr); // Iterator end() trỏ tới nullptr
    }

    template<class U>
    friend ostream& operator<<(ostream& os, const SLList<U>& ll);

    void insert(Iterator position, const T& value) {
        if (position == begin()) {
            AddHead(value);
            return;
        }

        Node* newNode = CreateNode(value);
        Node* p = _pHead;

        while (p != nullptr && p->_pNext != position.current_node) {
            p = p->_pNext;
        }

        if (p != nullptr) {
            newNode->_pNext = p->_pNext;
            p->_pNext = newNode;
            if (newNode->_pNext == nullptr) {
                _pTail = newNode;
            }
            _n++;
        }

    }

    const T& operator[](int index) const {
        if (index < 0 || index >= _n) {
            throw out_of_range("Index out of range");
        }
        Node* p = _pHead;
        for (int i = 0; i < index; ++i) {
            p = p->_pNext;
        }
        return p->_info;
    }

    T& operator[](int index) {
        if (index < 0 || index >= _n) {
            throw out_of_range("Index out of range");
        }
        Node* p = _pHead;
        for (int i = 0; i < index; ++i) {
            p = p->_pNext;
        }
        return p->_info;
    }

    void push_back(const T& value) {
        AddTail(value);
    }

    SLList(const SLList<T>& other) : _pHead(nullptr), _pTail(nullptr), _n(0) {
        Node* current = other._pHead;
        while (current != nullptr) {
            this->AddTail(current->_info);
            current = current->_pNext;
        }
    }

    SLList<T>& operator=(const SLList<T>& other) {
        if (this == &other) {
            return *this;
        }

        this->clear();

        Node* current = other._pHead;
        while (current != nullptr) {
            this->AddTail(current->_info);
            current = current->_pNext;
        }

        return *this;
    }

    int size() const {
        return _n;
    }

    bool empty() const {
        return _n == 0;
    }

    T& back() {
        if (_pTail == nullptr) {

            throw std::out_of_range("Calling back() on empty list");
        }
        return _pTail->_info;
    }

    const T& back() const {
        if (_pTail == nullptr) {
            throw std::out_of_range("Calling back() on empty list");
        }
        return _pTail->_info;
    }

    void push_front(const T& value) { AddHead(value); }
    void pop_front() { RemoveHead(); }
    void pop_back() { RemoveTail(); }

};

template<class T>
SLList<T>::SLList()
{
    _pHead = _pTail = nullptr;
    _n = 0;
}

template<class T>
SLList<T>::~SLList()
{
    clear();
}

template<class T>
void SLList<T>::clear()
{
    Node* current = _pHead;
    while (current != nullptr) {
        Node* next = current->_pNext;
        delete current;
        current = next;
    }
    _pHead = nullptr;
    _pTail = nullptr;
    _n = 0;
}

template<class T>
void SLList<T>::AddTail(const T& value)
{
    Node* node = CreateNode(value);
    if (node == nullptr)
    {
        return;
    }

    if (_pHead == nullptr)
    {
        _pHead = _pTail = node;
        _n++;
        return;
    }

    _pTail->_pNext = node;
    _pTail = node;
    _n++;
}

template<class T>
void SLList<T>::AddHead(const T& value)
{
    Node* newNode = CreateNode(value);
    if (_pHead == nullptr) {
        _pHead = _pTail = newNode;
    }
    else {
        newNode->_pNext = _pHead;
        _pHead = newNode;
    }
    _n++;
}

template<class T>
void SLList<T>::RemoveHead()
{
    if (_pHead == nullptr) return;
    Node* temp = _pHead;
    _pHead = _pHead->_pNext;
    delete temp;
    if (_pHead == nullptr) _pHead = _pTail = nullptr;
    _n--;
}

template<class T>
void SLList<T>::RemoveTail()
{
    if (_pHead == nullptr) return;
    if (_pHead == _pTail) {
        delete _pHead;
        _pHead = _pTail = nullptr;
        _n = 0;
        return;
    }

    Node* p = _pHead;
    while (p->_pNext != _pTail) {
        p = p->_pNext;
    }
    delete _pTail;
    _pTail = p;
    _n--;
}

template<class T>
class SLList<T>::Iterator
{
public:
    using iterator_category = std::forward_iterator_tag;
    using difference_type = std::ptrdiff_t;
    using value_type = T;
    using pointer = T*;
    using reference = T&;

private:
    Node* current_node;
    Iterator(Node* node) : current_node(node) {}
    friend class SLList<T>;

public:
    Iterator() : current_node(nullptr) {}

    reference operator*() const {
        if (!current_node)
        {
            throw out_of_range("Dereferencing end() or null iterator");
        }
        return current_node->_info;
    }

    pointer operator->() const {
        if (!current_node)
        {
            throw out_of_range("Accessing member via end() or null iterator");
        }
        return &(current_node->_info);
    }

    Iterator& operator++() {
        if (current_node)
        {
            current_node = current_node->_pNext;
        }
        else
        {
            throw out_of_range("Incrementing end() or null iterator");
        }
        return *this;
    }

    Iterator operator++(int) {
        if (!current_node)
        {
            throw out_of_range("Incrementing end() or null iterator");
        }
        Iterator temp = *this;
        current_node = current_node->_pNext;
        return temp;
    }

    bool operator==(const Iterator& other) const {
        return current_node == other.current_node;
    };
    bool operator!=(const Iterator& other) const {
        return !(*this == other);
    };
};

template<class T>
ostream& operator<<(ostream& os, const SLList<T>& ll)
{
    typename SLList<T>::Node* node = ll._pHead;
    while (node != nullptr)
    {
        os << node->_info << ", ";
        node = node->_pNext;
    }
    return os;
}

void fnAction(int val)
{
    cout << val << ", ";
}

void fnAction2(int& val)
{
    val *= 2;
}

bool fnPredict(int val)
{
    const int threshold = 3;
    return val > threshold;
}
