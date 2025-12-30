#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

// 1. CLASS CITEM (COMPONENT) - Lớp cha chung
class CItem {
protected:
    string _name;
    bool _isHidden;
    bool _isReadOnly;
public:
    CItem(string name) : _name(name), _isHidden(false), _isReadOnly(false) {}
    virtual ~CItem() {}

    // Các phương thức thuần ảo hoặc ảo để lớp con định nghĩa
    virtual long GetSize() const = 0;
    virtual void Print(bool isPrintHiddenItems) const = 0;
    
    // Tìm kiếm đệ quy
    virtual CItem* FindByName(const string& name) {
        // Mặc định (với File) chỉ so sánh tên chính nó
        if (_name == name) return this;
        return nullptr;
    }

    // Set Hidden: isApplyToChildren chỉ có ý nghĩa với Folder
    virtual void SetHidden(bool isHidden, bool isApplyToChildren) {
        _isHidden = isHidden;
    }

    string GetName() const { return _name; }
    bool IsHidden() const { return _isHidden; }
};

// 2. CLASS CFILE (LEAF) - Đại diện tập tin
class CFile : public CItem {
private:
    long _size;

public:
    CFile(string name, long size) : CItem(name), _size(size) {}

    long GetSize() const override {
        return _size;
    }

    void Print(bool isPrintHiddenItems) const override {
        // Nếu file bị ẩn và không yêu cầu in file ẩn -> return
        if (_isHidden && !isPrintHiddenItems) return;

        cout << "File: " << _name 
             << " | Size: " << _size 
             << " | Hidden: " << (_isHidden ? "True" : "False") 
             << endl;
    }

    // Với File, FindByName và SetHidden dùng implementation mặc định của CItem là đủ.
};

// 3. CLASS CFOLDER (COMPOSITE) - Đại diện thư mục
class CFolder : public CItem {
private:
    vector<CItem*> _items; // Chứa các con (File hoặc Folder)

public:
    CFolder(string name) : CItem(name) {}

    // Add: Thêm item vào folder
    void Add(CItem* item) {
        _items.push_back(item);
    }

    // GetSize: Tổng kích thước của tất cả item bên trong
    long GetSize() const override {
        long totalSize = 0;
        for (const auto& item : _items) {
            totalSize += item->GetSize();
        }
        return totalSize;
    }

    // Print: In folder và nội dung bên trong
    void Print(bool isPrintHiddenItems) const override {
        if (_isHidden && !isPrintHiddenItems) return;

        cout << "Folder: " << _name 
             << " | Size: " << GetSize() 
             << " | Hidden: " << (_isHidden ? "True" : "False") 
             << endl;
        
        // In các phần tử con
        for (const auto& item : _items) {
            // Logic in đệ quy: item con tự quyết định có in hay không dựa vào isPrintHiddenItems
            item->Print(isPrintHiddenItems); 
        }
    }

    // RemoveByName: Xóa item khỏi danh sách quản lý (nhưng không delete vùng nhớ vì hàm main dùng biến stack)
    CItem* RemoveByName(const string& name) {
        for (auto it = _items.begin(); it != _items.end(); ++it) {
            if ((*it)->GetName() == name) {
                CItem* removedItem = *it;
                _items.erase(it);
                return removedItem;
            }
        }
        return nullptr;
    }

    // FindByName: Tìm kiếm đệ quy (Tìm trong ds con, nếu con là Folder thì tìm tiếp trong đó)
    CItem* FindByName(const string& name) override {
        // 1. Kiểm tra chính nó (ít khi dùng nhưng cho đủ logic)
        if (_name == name) return this;

        // 2. Duyệt các con
        for (const auto& item : _items) {
            // Nếu tên của con trùng -> trả về con
            if (item->GetName() == name) return item;

            // Nếu con là Folder, gọi đệ quy FindByName của con
            // (Lưu ý: Cần ép kiểu hoặc tận dụng tính đa hình. Vì CItem có FindByName ảo nên gọi thẳng được)
            CItem* found = item->FindByName(name);
            if (found != nullptr) return found;
        }
        return nullptr;
    }

    // SetHidden: Xử lý logic áp dụng cho con
    void SetHidden(bool isHidden, bool isApplyToChildren) override {
        _isHidden = isHidden; // Set cho bản thân folder

        if (isApplyToChildren) {
            for (const auto& item : _items) {
                // Đệ quy set cho các con, và cũng yêu cầu các con áp dụng tiếp xuống dưới
                item->SetHidden(isHidden, true); 
            }
        }
    }
};

// 4. HAM MAIN
int main() {
    //Tạo các đối tượng
    CFolder C("C");
    CFolder System("System");
    CFolder Windows("Windows");
    
    //Tạo File
    CFile a_txt("a.txt", 123);
    CFile b_doc("b.doc", 456);

    //Add file vào folder
    System.Add(&a_txt);     // System chứa a.txt
    Windows.Add(&b_doc);    // Windows chứa b.doc

    //Add folder vào folder
    C.Add(&System);         // C chứa System
    C.Add(&Windows);        // C chứa Windows (như hình 1214 dòng đầu)

    //In cây thư mục ban đầu
    cout << "--- Content of folder C ->" << endl;
    bool isPrintHiddenItems = false;
    C.Print(isPrintHiddenItems); // In tất cả (lúc này chưa có gì hidden)

    //Xóa folder System
    cout << "\n--- Removing System..." << endl;
    CItem* p = C.RemoveByName("System"); // p trỏ tới folder System vừa bị tách ra
    cout << "Content of folder C after removing folder System ->" << endl;
    C.Print(false);

    //Tìm kiếm b.doc (đang nằm sâu trong Windows)
    cout << "\n--- Finding b.doc..." << endl;
    p = C.FindByName("b.doc"); 
    if (p != nullptr) {
        cout << "b.doc is found in folder C" << endl;
    } else {
        cout << "b.doc is not found" << endl;
    }

    // Tìm kiếm a.txt (đã bị xóa cùng System)
    cout << "\n--- Finding a.txt..." << endl;
    p = C.FindByName("a.txt");
    if (p != nullptr) {
        cout << "a.txt is found" << endl;
    } else {
        cout << "a.txt is not found" << endl; // Mong đợi dòng này
    }

    //Tìm folder Windows để set Hidden
    cout << "\n--- Setting Hidden for Windows..." << endl;
    p = C.FindByName("Windows");
    if (p != nullptr) {
        bool isHidden = true;
        bool isAlsoApplyToChildren = true;
        
        //Set Hidden cho Windows và toàn bộ con của nó
        p->SetHidden(isHidden, isAlsoApplyToChildren);
        
        //Kiểm tra kết quả in khi hidden
        cout << "Folder Windows is hidden. Content of folder C (showing visible only) ->" << endl;
        C.Print(false); // Sẽ không thấy Windows và b.doc
        
        cout << "Content of folder C (showing hidden items) ->" << endl;
        C.Print(true);  // Sẽ thấy Windows (Hidden: True) và b.doc (Hidden: True)
    } else {
        cout << "Folder Windows is not found" << endl;
    }

    // Pause để xem kết quả
    cout << endl;
    system("pause");
    return 0;
}
