#include "AllInOne.h"
#include <map>

// =========================================================================
// KHU VỰC 1: SINGLETON - KHO HÀNG (WAREHOUSE)
// [MAP TO MAIN]: Warehouse::GetInstance()->Import("Motor", 5);
// [TƯ DUY]: Quản lý số lượng tồn kho. Khi lắp ráp (Add), phải trừ kho.
// =========================================================================
class Warehouse : public Singleton<Warehouse> {
    friend class Singleton<Warehouse>;
    map<string, int> _inventory; // Lưu: Tên linh kiện -> Số lượng

    Warehouse() {}

public:
    // [MAP TO MAIN]: Nhập hàng vào kho
    void Import(string name, int qty) {
        _inventory[name] += qty;
    }

    // [LOGIC QUAN TRỌNG]: Kiểm tra và Xuất kho
    // Nếu hết hàng -> Ném lỗi -> Không cho Add vào cụm máy
    void CheckAndExport(string name) {
        // Nếu mặt hàng có trong danh sách quản lý
        if (_inventory.find(name) != _inventory.end()) {
            if (_inventory[name] > 0) {
                _inventory[name]--; // Trừ đi 1 cái
            } else {
                throw AppError("HET HANG TRONG KHO: " + name);
            }
        }
        // Lưu ý: Nếu tên không có trong kho (vd: tên của Cụm máy), ta bỏ qua.
    }
};

// =========================================================================
// KHU VỰC 2: INTERFACE TRUNG GIAN (PART)
// [TƯ DUY]: Tại sao cần class này?
// - BaseObject chỉ có GetValue() (ta dùng làm Giá tiền).
// - Bài này cần thêm Trọng lượng (Weight).
// - BaseObject chưa có GetWeight(), nên ta phải mở rộng.
// =========================================================================
class Part : public BaseObject {
public:
    Part(string name) : BaseObject(name) {}

    // Hàm ảo mới: Lấy trọng lượng
    virtual double GetWeight() const { return 0; }
};

// =========================================================================
// KHU VỰC 3: CLASS LÁ - CHI TIẾT ĐƠN (SIMPLE PART)
// [MAP TO MAIN]: PartFactory::Create("Motor | 500 | 10")
// =========================================================================
class SimplePart : public Part {
private:
    double _price;
    double _weight;
public:
    SimplePart(string name, double price, double weight)
        : Part(name), _price(price), _weight(weight) {}

    // [MAP TO MAIN]: GetValue là Giá tiền
    double GetValue() const override { return _price; }

    // [MAP TO MAIN]: GetWeight là Trọng lượng
    double GetWeight() const override { return _weight; }

    void Xuat(ostream& os) const override {
        os << "[Part] " << _name << " ($" << _price << " | " << _weight << "kg)";
    }
};

// =========================================================================
// KHU VỰC 4: COMPOSITE - CỤM MÁY / CHI TIẾT PHỨC (COMPLEX PART)
// [MAP TO MAIN]: ComplexPart* engine = new ComplexPart("Cum Dong Co");
// [TƯ DUY]: Đây là nơi chứa "Bẫy Formula".
// =========================================================================
class ComplexPart : public Part, public CompositeNode {
public:
    ComplexPart(string name) : Part(name) {}

    // [LOGIC BẪY 1]: Tính GIÁ THÀNH
    // Công thức: Tổng giá linh kiện + 20% phí lắp ráp
    double GetValue() const override {
        double subTotal = 0;
        for (auto child : *this) {
            subTotal += child->GetValue(); // BaseObject::GetValue đa hình
        }
        // Nhân hệ số 1.2 (Tăng 20%)
        return subTotal * 1.2;
    }

    // [LOGIC BẪY 2]: Tính TRỌNG LƯỢNG
    // Công thức: Tổng trọng lượng con + 10% phụ kiện nối
    double GetWeight() const override {
        double subTotal = 0;
        for (auto child : *this) {
            // [BẪY ÉP KIỂU]: Phải ép sang Part* mới gọi được GetWeight()
            Part* p = dynamic_cast<Part*>(child);
            if (p) subTotal += p->GetWeight();
        }
        // Nhân hệ số 1.1 (Tăng 10%)
        return subTotal * 1.1;
    }

    // [LOGIC KHO HÀNG]: Khi thêm linh kiện vào cụm -> Phải trừ kho
    void Add(BaseObject* obj) override {
        if (obj) {
            // Gọi Singleton kiểm tra tên linh kiện
            Warehouse::GetInstance()->CheckAndExport(obj->GetName());
            
            // Nếu không lỗi (còn hàng) thì mới Add
            CompositeNode::Add(obj);
        }
    }

    void Xuat(ostream& os) const override {
        os << "[CUM MAY] " << _name 
           << " { Gia: $" << GetValue() 
           << " | Nang: " << GetWeight() << "kg }";
        for (auto c : *this) os << "\n    + " << *c;
    }
};

// =========================================================================
// KHU VỰC 5: FACTORY - TẠO LINH KIỆN
// [MAP TO MAIN]: Create("Motor | 500 | 10") -> Tên | Giá | Nặng
// =========================================================================
class PartFactory {
public:
    static BaseObject* Create(string s) {
        // [XỬ LÝ CHUỖI]: Cắt bằng dấu gạch đứng '|'
        vector<string> p = Utils::Split(s, '|');
        
        if (p.size() >= 3) {
            string name = p[0];
            double price = Utils::ToNum(p[1]);
            double weight = Utils::ToNum(p[2]);
            return new SimplePart(name, price, weight);
        }
        return nullptr;
    }
};

// =========================================================================
// KHU VỰC 6: HÀM MAIN (ĐỀ BÀI MẪU)
// =========================================================================
/*
int main() {
    try {
        cout << "--- HE THONG QUAN LY CHI TIET MAY ---" << endl;

        // 1. Singleton: Setup Kho hàng
        Warehouse::GetInstance()->Import("Motor", 5);
        Warehouse::GetInstance()->Import("BanhRang", 10);

        // 2. Composite: Cụm động cơ
        ComplexPart* engine = new ComplexPart("Cum Dong Co");

        // 3. Factory: Tạo linh kiện
        // Motor: 500$, 10kg
        BaseObject* p1 = PartFactory::Create("Motor | 500 | 10"); 
        
        // Bánh răng: 50$, 0.5kg
        BaseObject* p2 = PartFactory::Create("BanhRang | 50 | 0.5");

        // 4. Test Logic Add (Trừ kho) & Tính toán (Formula Trap)
        engine->Add(p1); // Kho Motor còn 4
        engine->Add(p2); // Kho BanhRang còn 9
        
        // In kết quả
        // Giá = (500 + 50) * 1.2 = 660
        // Nặng = (10 + 0.5) * 1.1 = 11.55
        cout << *engine << endl;

        // 5. Test Exception Kho Hàng
        cout << "\n>> THU LAY QUA SO LUONG..." << endl;
        // Cố tình add thêm 10 cái Bánh răng (Kho chỉ còn 9)
        for(int i=0; i<10; i++) {
             engine->Add(PartFactory::Create("BanhRang | 50 | 0.5"));
        }

    } catch (exception& e) {
        cout << "LOI KHO HANG: " << e.what() << endl;
    }
    return 0;
}
*/
