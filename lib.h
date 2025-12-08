#ifndef LIB_H
#define LIB_H

#include <iostream>
#include <string>
#include <vector>
#include <numeric>

using namespace std;

// 1. Abstract Base Class (Component trong Composite Pattern)
[cite_start]// [cite: 5] Thuộc tính chính là tên.
class BaseItem {
protected:
    string name;

public:
    BaseItem(string n) : name(n) {}
    virtual ~BaseItem() {}

    // Phương thức thuần ảo để tính kích thước
    virtual int getSize() const = 0;
    string getName() const { return name; }
};

// 2. Class File
[cite_start]// [cite: 7] File có thông tin kích thước, loại tệp.
class File : public BaseItem {
private:
    int size;
    string type; // "binary" hoặc "text"

public:
    File(string n, int s, string t);
    int getSize() const override;
};

// 3. Class Folder
[cite_start]// [cite: 4] Folder chứa File, Shortcut, Folder con.
class Folder : public BaseItem {
private:
    vector<BaseItem*> children; // Danh sách các thành phần con

public:
    Folder(string n);
    ~Folder(); // Cần destructor để xóa bộ nhớ các con

    void add(BaseItem* item);

    [cite_start]// [cite: 11] Kích thước folder = tổng kích thước các phần tử con.
        int getSize() const override;
};

// 4. Class Shortcut
[cite_start]// [cite: 8] Shortcut trỏ đến nguồn (Folder hoặc File).
class Shortcut : public BaseItem {
private:
    BaseItem* target; // Con trỏ tham chiếu đến đối tượng gốc (không sở hữu)

public:
    Shortcut(string n, BaseItem* src);

    [cite_start]// [cite: 11] Kích thước shortcut là 1KB.
        int getSize() const override;
};

// 5. Class Disk
[cite_start]// [cite: 2, 6] Disk chứa Folder, File, Shortcut và có dung lượng.
class Disk {
private:
    string name;
    int capacity;
    vector<BaseItem*> content; // Disk hoạt động như một Root Folder

public:
    Disk(string n, int cap);
    ~Disk();

    void add(BaseItem* item);

    [cite_start]// [cite: 10] Phương thức lấy dung lượng chưa sử dụng.
        int getFreeSpace() const;
};

#endif
