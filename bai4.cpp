#include "AllInOne.h"
#include <map>

// =========================================================================
// KHU VỰC 1: SINGLETON - QUẢN LÝ KHUYẾN MÃI (PROMOTION MANAGER)
// [TƯ DUY]:
// - Lưu trữ các mã giảm giá, mức giảm, và ngày hết hạn.
// - Cung cấp hàm GetDiscountRate(category) để Product tự hỏi mức giảm giá.
// =========================================================================

struct PromoInfo {
    double rate;    // Mức giảm (vd: 0.1 = 10%)
    Date expire;    // Ngày hết hạn
    string targetCategory; // Áp dụng cho loại nào (Rỗng = Tất cả)
};

class PromotionManager : public Singleton<PromotionManager> {
    friend class Singleton<PromotionManager>;
    map<string, PromoInfo> _codes; // Danh sách mã đã nạp
    string _activeCode;            // Mã người dùng đang nhập
    Date _today;                   // Ngày hiện tại của hệ thống

    PromotionManager() {}

public:
    void SetCurrentDate(Date d) { _today = d; }

    // Đăng ký mã mới vào hệ thống
    void AddCode(string code, double rate, Date expire, string cat = "") {
        _codes[code] = {rate, expire, cat};
    }

    // Người dùng nhập mã giảm giá
    void ApplyCode(string code) {
        if (_codes.find(code) == _codes.end()) {
            cout << "Warning: Ma khong ton tai!" << endl;
            return;
        }
        // Check ngày hết hạn (Dùng operator<= mới thêm trong AllInOne)
        if (_codes[code].expire < _today) {
            cout << "Warning: Ma da het han!" << endl;
            return;
        }
        _activeCode = code; // Mã hợp lệ -> Lưu lại
    }

    // [LOGIC TÍNH TIỀN]: Hàm này trả về % giảm giá cho một món hàng cụ thể
    double GetDiscountRate(string productCategory) {
        if (_activeCode.empty()) return 0; // Chưa nhập mã -> Không giảm

        PromoInfo info = _codes[_activeCode];
        
        // Nếu mã này áp dụng cho tất cả HOẶC đúng category của sản phẩm
        // (Ví dụ: Mã cho Electronics thì Food không được giảm)
        if (info.targetCategory == "" || info.targetCategory == productCategory) {
            return info.rate;
        }
        return 0;
    }
};

// =========================================================================
// KHU VỰC 2: CLASS LÁ - SẢN PHẨM (PRODUCT)
// [MAP TO MAIN]: ProductFactory::Create("Laptop | 1500 | Electronics")
// =========================================================================

class Product : public BaseObject {
private:
    double _price;
    string _category; // Danh mục: Electronics, Food...
public:
    Product(string name, double price, string cat) 
        : BaseObject(name), _price(price), _category(cat) {}

    // Override hàm mới trong AllInOne để trả về Category
    string GetCategory() const override { return _category; }

    // [LOGIC TÍNH TIỀN THÔNG MINH]:
    // Giá thực tế = Giá gốc * (1 - %Giảm giá từ Singleton)
    double GetValue() const override {
        double discount = PromotionManager::GetInstance()->GetDiscountRate(_category);
        return _price * (1.0 - discount);
    }

    void Xuat(ostream& os) const override {
        // In giá gốc và giá sau giảm (nếu có)
        os << "[SP] " << _name << " | " << _category << " | $" << GetValue();
        if (PromotionManager::GetInstance()->GetDiscountRate(_category) > 0) {
            os << " (Da giam)";
        }
    }
};

// =========================================================================
// KHU VỰC 3: COMPOSITE - GIỎ HÀNG / COMBO (BUNDLE)
// [TƯ DUY]: Kế thừa CompositeNode để chứa sản phẩm. 
// Thêm tính năng Sort và Find.
// =========================================================================

class Bundle : public BaseObject, public CompositeNode {
public:
    Bundle(string name) : BaseObject(name) {}

    // Tính tổng tiền (Cộng dồn giá trị thực tế của con)
    double GetValue() const override {
        double total = 0;
        for (auto child : *this) total += child->GetValue();
        return total;
    }

    // [TÍNH NĂNG SORT]: Sắp xếp con theo giá tăng dần
    void SortByPrice() {
        // Gọi hàm SortChildren mới thêm trong AllInOne
        // Dùng Lambda function để so sánh 2 BaseObject*
        SortChildren([](BaseObject* a, BaseObject* b) {
            return a->GetValue() < b->GetValue();
        });
    }

    // [TÍNH NĂNG SEARCH]: Tìm kiếm đệ quy theo tên
    BaseObject* FindRecursive(string keyword) {
        // Duyệt qua các con
        for (auto child : *this) {
            // Check tên con
            if (child->GetName().find(keyword) != string::npos) {
                return child; // Tìm thấy
            }
            
            // Nếu con là Bundle, tìm sâu vào trong (Đệ quy)
            // Cần ép kiểu vì BaseObject không có hàm FindRecursive
            Bundle* subBundle = dynamic_cast<Bundle*>(child);
            if (subBundle) {
                BaseObject* res = subBundle->FindRecursive(keyword);
                if (res) return res;
            }
        }
        return nullptr; // Không thấy
    }

    void Xuat(ostream& os) const override {
        os << "[BUNDLE] " << _name << " { Total: $" << GetValue() << " }";
        for (auto c : *this) os << "\n    + " << *c;
    }
};

// =========================================================================
// KHU VỰC 4: FACTORY - XỬ LÝ 3 THAM SỐ
// [MAP TO MAIN]: "Name | Price | Category"
// =========================================================================

class ProductFactory {
public:
    static BaseObject* Create(string s) {
        // Dấu phân cách là '|'
        vector<string> p = Utils::Split(s, '|');
        
        if (p.size() >= 3) {
            string name = p[0];
            double price = Utils::ToNum(p[1]);
            string cat = p[2];
            return new Product(name, price, cat);
        }
        return nullptr;
    }
};

// =========================================================================
// KHU VỰC 5: HÀM MAIN (ĐỀ BÀI FINAL BOSS)
// =========================================================================
/*
int main() {
    try {
        cout << "--- SHOPPING CART SYSTEM ---" << endl;

        // 1. Setup Singleton: Ngày và Mã KM
        Date today(30, 12, 2025);
        PromotionManager::GetInstance()->SetCurrentDate(today);
        
        // TET2026: Giảm 10% cho đồ điện tử, Hết hạn 1/1/2026 (Còn hạn)
        PromotionManager::GetInstance()->AddCode("TET2026", 0.1, Date(1, 1, 2026), "Electronics");
        
        // NOEL: Giảm 50%, Hết hạn 25/12/2025 (Đã hết hạn -> Ko tác dụng)
        PromotionManager::GetInstance()->AddCode("NOEL", 0.5, Date(25, 12, 2025), "Food");

        // Áp dụng mã TET2026
        PromotionManager::GetInstance()->ApplyCode("TET2026");

        // 2. Tạo Giỏ hàng & Combo
        Bundle* cart = new Bundle("Gio Hang Cuoi Nam");
        Bundle* techPack = new Bundle("Combo Cong Nghe");

        // 3. Thêm hàng hóa
        // Laptop: Electronics -> Được giảm 10% của 1500 -> Còn 1350
        techPack->Add(ProductFactory::Create("Laptop Dell | 1500 | Electronics"));
        
        // Chuột: Electronics -> Được giảm 10% của 50 -> Còn 45
        techPack->Add(ProductFactory::Create("Mouse Logi | 50 | Electronics"));

        cart->Add(techPack);
        
        // Bánh: Food -> Không được giảm (Mã TET chỉ cho Electronics) -> 20
        cart->Add(ProductFactory::Create("Banh Danisa | 20 | Food"));

        // 4. In kết quả tính tiền
        // Tổng: 1350 + 45 + 20 = 1415
        cout << *cart << endl; 

        // 5. Test Sắp xếp (Sort)
        cout << "\n--- SAP XEP COMBO THEO GIA ---" << endl;
        techPack->SortByPrice(); // Mouse (45) lên trước, Laptop (1350) xuống sau
        cout << *techPack << endl;

        // 6. Test Tìm kiếm (Find)
        cout << "\n--- TIM KIEM 'Dell' ---" << endl;
        BaseObject* found = cart->FindRecursive("Dell");
        if (found) cout << "Tim thay: " << *found << endl;
        else cout << "Khong tim thay!" << endl;

    } catch (exception& e) {
        cout << "LOI: " << e.what() << endl;
    }
    return 0;
}
*/
