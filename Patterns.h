#include "Patterns.h"

// Bạn chỉ cần kế thừa Singleton<SystemLog> là xong!
// Không cần khai báo static instance dòng dài dòng nữa.
class SystemLog : public Singleton<SystemLog> {
    // Để Singleton<SystemLog> truy cập được constructor private của class này
    friend class Singleton<SystemLog>; 
private:
    SystemLog() {} // Private constructor
public:
    void Log(string msg) {
        cout << "[LOG]: " << msg << endl;
    }
};

// Dùng trong main:
// SystemLog::GetInstance()->Log("Chuong trinh bat dau");

#include "Core.h"
#include "Patterns.h"

// 1. Class Thư mục kế thừa BaseObject (để đa hình) VÀ CompositeNode (để quản lý list)
class Folder : public BaseObject, public CompositeNode {
private:
    string name;
public:
    Folder(string n) : name(n) {}

    // Implement hàm Nhap/Xuat của BaseObject
    void Nhap(istream& is) override { /*...*/ }
    void Xuat(ostream& os) const override {
        os << "Folder: " << name << endl;
        
        // TẬN DỤNG COMPOSITE NODE: Duyệt danh sách con có sẵn
        for (auto child : _children) { 
            // Gọi đệ quy in ra các con
            child->Xuat(os); 
        }
    }

    // Hàm tính dung lượng (Đệ quy)
    int GetSize() {
        int total = 0;
        for (auto child : _children) {
            // Ép kiểu để gọi hàm GetSize riêng (nếu BaseObject ko có GetSize)
            // Hoặc tốt nhất là khai báo virtual GetSize() trong BaseObject
             total += child->GetSize(); // Giả sử BaseObject đã có hàm ảo này
        }
        return total;
    }
};

// Class File (Lá)
class File : public BaseObject {
    // ... code bình thường ...
};
