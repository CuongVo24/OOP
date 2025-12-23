#include <iostream>
#include <vector>
#include <string>
#include <cmath>

using namespace std;

// === Component (Abstract) ===
class Component {
protected:
    string _name;
public:
    Component(const string& s) : _name(s) {}
    virtual ~Component() {}

    // Hàm thuần ảo lấy kích thước
    virtual int GetSize() = 0;

    void Print(ostream& os) {
        os << _name << " (" << GetSize() << ")" << endl;
    }
};

// === File (Lá - Leaf) ===
class File : public Component {
private:
    int _size;
public:
    File(const string& s, int size) : Component(s) {
        _size = abs(size);
    }

    int GetSize() override {
        return _size;
    }
};

// === Directory (Nhánh - Composite) ===
class Directory : public Component {
private:
    vector<Component*> _children; // Chứa cả File lẫn Directory con
public:
    Directory(const string& s) : Component(s) {}
    
    ~Directory() {
        // Directory chịu trách nhiệm hủy các thành phần con của nó
        for (auto c : _children) delete c;
    }

    void AddChild(Component* c) {
        _children.push_back(c);
    }

    // Tính tổng kích thước bằng cách đệ quy gọi GetSize của các con
    int GetSize() override {
        int total = 0;
        for (int i = 0; i < _children.size(); i++) {
            total += _children[i]->GetSize();
        }
        return total;
    }
};

int main() {
    cout << "\n--- VI DU 2: COMPOSITE PATTERN ---" << endl;
    
    Directory* dRoot = new Directory("Dir root");
    Directory* dChild = new Directory("Dir child");

    dChild->AddChild(new File("File 1", 123));
    dChild->AddChild(new File("File 2", 444));

    dRoot->AddChild(dChild);
    dRoot->AddChild(new File("File 3", 7899));

    // Gọi hàm Print của Root, GetSize sẽ đệ quy tính tổng
    dRoot->Print(cout);

    cout << endl;
    delete dRoot; // Destructor của Directory sẽ tự xóa hết con cháu
    return 0;
}
