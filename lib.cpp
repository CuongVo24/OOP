#include "lib.h"

// --- File Implementation ---
File::File(string n, int s, string t) : BaseItem(n), size(s), type(t) {}

int File::getSize() const {
    return size;
}

// --- Folder Implementation ---
Folder::Folder(string n) : BaseItem(n) {}

Folder::~Folder() {
    // [Clean Code] Folder sở hữu các item con, nên khi hủy Folder phải hủy các con
    // Lưu ý: Shortcut không sở hữu target, nên Shortcut hủy không ảnh hưởng target.
    for (BaseItem* item : children) {
        delete item;
    }
    children.clear();
}

void Folder::add(BaseItem* item) {
    children.push_back(item);
}

int Folder::getSize() const {
    int totalSize = 0;
    for (const auto& item : children) {
        totalSize += item->getSize(); // Đệ quy tính tổng size
    }
    return totalSize;
}

// --- Shortcut Implementation ---
Shortcut::Shortcut(string n, BaseItem* src) : BaseItem(n), target(src) {}

int Shortcut::getSize() const {
    return 1; [cite_start]// [cite: 11] Giả sử đơn vị là KB theo đề bài
}

// --- Disk Implementation ---
Disk::Disk(string n, int cap) : name(n), capacity(cap) {}

Disk::~Disk() {
    for (BaseItem* item : content) {
        delete item;
    }
    content.clear();
}

void Disk::add(BaseItem* item) {
    content.push_back(item);
}

[cite_start]// [cite: 10] Cài đặt phương thức getFreeSpace
int Disk::getFreeSpace() const {
    int usedSpace = 0;
    for (const auto& item : content) {
        usedSpace += item->getSize();
    }
    return capacity - usedSpace;
}
