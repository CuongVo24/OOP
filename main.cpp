#include <iostream>
#include "lib.h"

int main() {
    // 1. Tạo Disk C với dung lượng 1000 KB
    Disk* myDisk = new Disk("C:", 1000);

    // 2. Tạo một số File
    [cite_start]// [cite: 7] File có tên, kích thước, loại
        File* f1 = new File("Document.txt", 100, "text");
    File* f2 = new File("Image.png", 200, "binary");
    File* f3 = new File("System.exe", 50, "binary");

    // 3. Tạo Folder và thêm File vào
    [cite_start]// [cite: 4] Folder chứa File
        Folder* folder1 = new Folder("MyDocuments");
    folder1->add(f1);
    folder1->add(f2);

    // 4. Tạo Shortcut trỏ đến File và Folder
    [cite_start]// [cite: 8] Shortcut trỏ đến nguồn
        Shortcut* s1 = new Shortcut("LinkToImage", f2);
    Shortcut* s2 = new Shortcut("LinkToDocFolder", folder1);

    // 5. Thêm tất cả vào Disk
    [cite_start]// [cite: 3] Disk chứa Folder, File, Shortcut
        myDisk->add(folder1); // Folder chứa f1, f2
    myDisk->add(f3);      // File lẻ
    myDisk->add(s1);      // Shortcut
    myDisk->add(s2);      // Shortcut

    // 6. Tính toán và hiển thị
    [cite_start]// [cite: 11] Shortcut = 1KB, Folder = tổng con.
        cout << "--- System Info ---" << endl;
    cout << "Size of Folder 'MyDocuments' (100+200): " << folder1->getSize() << " KB" << endl;
    cout << "Size of Shortcut to Image: " << s1->getSize() << " KB" << endl; // Luôn là 1

    // Tổng used = Folder1(300) + f3(50) + s1(1) + s2(1) = 352
    // Free = 1000 - 352 = 648
    cout << "Free Space on Disk: " << myDisk->getFreeSpace() << " KB" << endl;

    // Dọn dẹp (Disk destructor sẽ xóa hết các con mà nó sở hữu)
    delete myDisk;
    // Lưu ý: f1, f2 nằm trong folder1, folder1 nằm trong Disk -> Disk xóa folder1 -> folder1 xóa f1,f2.
    // Tuy nhiên f3, s1, s2 được add trực tiếp vào Disk -> Disk sẽ xóa chúng.
    // Logic quản lý bộ nhớ ở đây giả định quyền sở hữu thuộc về container cha (Disk/Folder).

    return 0;
}
