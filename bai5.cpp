#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

// === Abstract Component ===
class ThanhPhan {
protected:
    string ten;
public:
    ThanhPhan(string name) : ten(name) {}
    virtual ~ThanhPhan() {}

    virtual int TinhKichThuoc() = 0;
    virtual int DemSoLuong() = 0; // Đếm số lượng tập tin + thư mục con
    virtual void Xuat(int level = 0) = 0; // level dùng để thụt đầu dòng (indent)
    
    string GetTen() { return ten; }
    
    // Helper để in thụt đầu dòng cho đẹp
    void Indent(int level) {
        for(int i=0; i<level; i++) cout << "\t";
    }
};

// === Leaf: Tập tin ===
class TapTin : public ThanhPhan {
private:
    int kichThuoc;
public:
    TapTin(string name, int size) : ThanhPhan(name), kichThuoc(size) {}

    int TinhKichThuoc() override { return kichThuoc; }

    int DemSoLuong() override { return 1; } // 1 tập tin tính là 1

    void Xuat(int level) override {
        Indent(level);
        cout << ten << " (" << kichThuoc << " B)" << endl;
    }
};

// === Composite: Thư mục ===
class ThuMuc : public ThanhPhan {
private:
    vector<ThanhPhan*> dsCon;
public:
    ThuMuc(string name) : ThanhPhan(name) {}

    ~ThuMuc() {
        for (auto tp : dsCon) delete tp;
    }

    void ThemThanhPhan(ThanhPhan* tp) {
        dsCon.push_back(tp);
    }

    int TinhKichThuoc() override {
        int tong = 0;
        for (auto tp : dsCon) tong += tp->TinhKichThuoc();
        return tong;
    }

    int DemSoLuong() override {
        int tong = 0;
        for (auto tp : dsCon) {
            // Đếm bản thân nó và đệ quy vào trong
            tong += tp->DemSoLuong(); 
        }
        return tong; // Trả về tổng số file và folder con bên trong
    }

    void Xuat(int level) override {
        Indent(level);
        cout << "[" << ten << "]" << endl;
        for (auto tp : dsCon) {
            tp->Xuat(level + 1); // Tăng level thụt đầu dòng cho con
        }
    }

    // Hàm tìm kiếm thư mục theo tên (Hỗ trợ câu hỏi tìm thư mục)
    ThuMuc* TimThuMuc(string name) {
        if (this->ten == name) return this;
        
        for (auto tp : dsCon) {
            // Dùng dynamic_cast để kiểm tra xem con có phải là Thư mục không
            ThuMuc* tm = dynamic_cast<ThuMuc*>(tp);
            if (tm) {
                ThuMuc* result = tm->TimThuMuc(name);
                if (result) return result;
            }
        }
        return nullptr;
    }
};

int main() {
    cout << "--- BAI 5: CAY THU MUC ---" << endl;

    // 1. Tạo cây thư mục như hình đề bài
    ThuMuc* root = new ThuMuc("C:");
    
    // Nhanh 1: Bai tap
    ThuMuc* baiTap = new ThuMuc("Bai tap");
    baiTap->ThemThanhPhan(new TapTin("BT1.doc", 123456));
    
    ThuMuc* baiTapC = new ThuMuc("Bai tap C");
    baiTapC->ThemThanhPhan(new TapTin("BT2.cpp", 1280));
    baiTap->ThemThanhPhan(baiTapC);
    
    baiTap->ThemThanhPhan(new TapTin("BT2.xls", 23456));

    // Nhanh 2: Ly thuyet
    ThuMuc* lyThuyet = new ThuMuc("Ly thuyet");
    lyThuyet->ThemThanhPhan(new TapTin("Ch1.ppt", 34567));

    // Gan vao Root
    root->ThemThanhPhan(baiTap);
    root->ThemThanhPhan(lyThuyet);

    // 2. Xuất cây thư mục
    cout << "\n>>> CAU TRUC CAY:" << endl;

    root->Xuat(0); // Truyền số 0 để bắt đầu in từ lề trái ngoài cùng

    // 3. Đếm số tập tin và thư mục trong một thư mục nào đó (Ví dụ: Bai tap)
    cout << "\n>>> DEM SO LUONG TRONG 'Bai tap':" << endl;
    cout << "So luong: " << baiTap->DemSoLuong() << " (bao gom file va folder con)" << endl;

    // 4. Tính kích thước một thư mục có tên cho trước (Ví dụ: Bai tap C)
    string tenCanTim = "Bai tap C";
    cout << "\n>>> TINH KICH THUOC FOLDER: " << tenCanTim << endl;
    ThuMuc* ketQua = root->TimThuMuc(tenCanTim);
    
    if (ketQua) {
        cout << "Kich thuoc cua [" << tenCanTim << "]: " << ketQua->TinhKichThuoc() << " bytes" << endl;
    } else {
        cout << "Khong tim thay thu muc!" << endl;
    }

    delete root; // Dọn dẹp toàn bộ cây
    system("pause");
    return 0;
}
