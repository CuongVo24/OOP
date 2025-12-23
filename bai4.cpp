#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>

using namespace std;

// 1. Abstract Base Class: ChiTietMay
class ChiTietMay {
protected:
    string maSo;
public:
    ChiTietMay(string ma) : maSo(ma) {}
    virtual ~ChiTietMay() {}

    // Các hàm ảo thuần túy
    virtual float TrongLuong() = 0;
    virtual float GiaThanh() = 0;

    // Hàm ảo hỗ trợ in ấn (để dùng cho operator<<)
    virtual void Xuat(ostream& os) const {
        os << "Ma: " << maSo 
           << " | TL: " << (long)const_cast<ChiTietMay*>(this)->TrongLuong() // const_cast để gọi hàm non-const nếu cần
           << " | Gia: " << (long long)const_cast<ChiTietMay*>(this)->GiaThanh();
    }

    // Nạp chồng operator<< cho ChiTietMay để in từng chi tiết
    friend ostream& operator<<(ostream& os, const ChiTietMay& ct) {
        ct.Xuat(os);
        return os;
    }
};

// 2. Leaf Class: ChiTietDon
class ChiTietDon : public ChiTietMay {
private:
    float trongLuong;
    float giaThanh;
public:
    ChiTietDon(string ma, float tl, float gia) 
        : ChiTietMay(ma), trongLuong(tl), giaThanh(gia) {}

    float TrongLuong() override {
        return trongLuong;
    }

    float GiaThanh() override {
        return giaThanh;
    }
    
    void Xuat(ostream& os) const override {
        os << "Chi tiet don: ";
        ChiTietMay::Xuat(os);
    }
};

// 3. Composite Class: ChiTietPhuc
class ChiTietPhuc : public ChiTietMay {
private:
    vector<ChiTietMay*> dsCon;
public:
    ChiTietPhuc(string ma) : ChiTietMay(ma) {}

    ~ChiTietPhuc() {
        for (int i = 0; i < dsCon.size(); i++) delete dsCon[i];
        dsCon.clear();
    }

    // Hàm này KHÔNG có trong lớp cha, nên ở main phải ép kiểu mới gọi được
    void ThemChiTiet(ChiTietMay* ct) {
        dsCon.push_back(ct);
    }

    float TrongLuong() override {
        float total = 0;
        for (int i = 0; i < dsCon.size(); i++) {
            total += dsCon[i]->TrongLuong();
        }
        return total + (total * 0.1); // +10% phụ kiện
    }

    float GiaThanh() override {
        float total = 0;
        for (int i = 0; i < dsCon.size(); i++) {
            total += dsCon[i]->GiaThanh();
        }
        return total + (total * 0.2); // +20% công lắp
    }

    void Xuat(ostream& os) const override {
        os << "Chi tiet phuc: " << maSo << " Gom " << dsCon.size() << " chi tiet con.";
        // Tùy chọn: Có thể in đệ quy các con nếu muốn, nhưng đề bài chỉ cần xuất list ở CoMay
    }
};

// 4. Container Class: CoMay
class CoMay {
private:
    vector<ChiTietMay*> dsChiTiet;
public:
    CoMay() {}
    ~CoMay() {
        for (int i = 0; i < dsChiTiet.size(); i++) delete dsChiTiet[i];
        dsChiTiet.clear();
    }

    void ThemChiTietMay(ChiTietMay* ct) {
        dsChiTiet.push_back(ct);
    }

    float TrongLuong() {
        float total = 0;
        for (int i = 0; i < dsChiTiet.size(); i++) {
            total += dsChiTiet[i]->TrongLuong();
        }
        return total;
    }

    float GiaThanh() {
        float total = 0;
        for (int i = 0; i < dsChiTiet.size(); i++) {
            total += dsChiTiet[i]->GiaThanh();
        }
        return total;
    }

    // Nạp chồng toán tử << để in thông tin Cỗ máy như đề bài yêu cầu
    friend ostream& operator<<(ostream& os, CoMay& cm) {
        for (int i = 0; i < cm.dsChiTiet.size(); i++) {
            // Gọi operator<< của ChiTietMay (nhờ đa hình sẽ gọi đúng Xuat của con)
            os << *cm.dsChiTiet[i] << endl;
        }
        return os;
    }
};

// 5. MAIN FUNCTION
int main() {
    /// tao moi Co may
    CoMay* pcm = new CoMay();
    
    /// them chi tiet
    pcm->ThemChiTietMay(new ChiTietDon("CTD001", 120, 350000));

    /// tao Chi tiet phuc
    // Lưu ý: Khai báo là ChiTietMay* nhưng new ChiTietPhuc
    ChiTietMay* pctm = new ChiTietPhuc("CTP001"); 
    
    // Ép kiểu về ChiTietPhuc* để gọi hàm ThemChiTiet (vì hàm này không có ở lớp cha)
    ((ChiTietPhuc*)pctm)->ThemChiTiet(new ChiTietDon("CTD002", 50, 1280000));
    ((ChiTietPhuc*)pctm)->ThemChiTiet(new ChiTietDon("CTD003", 20, 10000));

    // Tạo chi tiết phức con lồng nhau (Ảnh 1180)
    ChiTietMay* pctm1 = new ChiTietPhuc("CTP002");
    ((ChiTietPhuc*)pctm1)->ThemChiTiet(new ChiTietDon("CTD004", 80, 170000));

    // Thêm phức con vào phức cha
    ((ChiTietPhuc*)pctm)->ThemChiTiet(pctm1);

    // Thêm phức cha vào cỗ máy
    pcm->ThemChiTietMay(pctm);
    
    pcm->ThemChiTietMay(new ChiTietDon("CTD005", 210, 2350000));
    pcm->ThemChiTietMay(new ChiTietDon("CTD006", 40, 50000));

    /// xuat thong tin Co may
    /// xuat danh sach cac chi tiet con
    // Ở đây dereference con trỏ pcm (*pcm) để gọi operator<<
    cout << "Thong tin co may: " << endl << *pcm << endl;

    /// xuat trong luong co may
    cout << "Trong luong: " << pcm->TrongLuong() << endl;

    /// xuat gia thanh co may
    cout << "Gia thanh: " << (long long)pcm->GiaThanh() << endl;

    cout << endl << endl;
    
    // Dọn dẹp bộ nhớ (Tốt cho thói quen, dù đề bài không ghi rõ đoạn này ở main)
    delete pcm; 
    
    system("pause");
    return 0;
}
