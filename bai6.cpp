#include "AllInOne.h"

// =========================================================================
// PHẦN 1: DECORATOR PATTERN (TOPPING CAFE)
// =========================================================================

// 1. Component Gốc (Ly nước) - Đã có BaseObject lo phần GetValue (Giá)
// Ta dùng BaseObject* làm chuẩn.

// 2. Concrete Component (Món chính)
class Coffee : public BaseObject {
    double _price;
public:
    Coffee(string name, double price) : BaseObject(name), _price(price) {}
    double GetValue() const override { return _price; } // Giá gốc
};

// 3. Decorator (Lớp bao bọc Topping)
// Mấu chốt: Decorator CŨNG LÀ BaseObject, nhưng nó CHỨA 1 BaseObject khác bên trong.
class ToppingDecorator : public BaseObject {
protected:
    BaseObject* _component; // Đối tượng bị bao bọc (VD: Ly Cafe)
    double _price;          // Giá topping
public:
    ToppingDecorator(BaseObject* component, string name, double price) 
        : BaseObject(name), _component(component), _price(price) {}

    // Tính giá = Giá món bên trong + Giá topping này
    double GetValue() const override {
        return _component->GetValue() + _price;
    }

    void Xuat(ostream& os) const override {
        _component->Xuat(os); // In món bên trong trước
        os << " + " << _name; // In thêm topping
    }
};

// =========================================================================
// PHẦN 2: STRATEGY PATTERN (KHUYẾN MÃI)
// =========================================================================

// 1. Interface Chiến lược
class IPromotionStrategy {
public:
    virtual double CalculateDiscount(double total) = 0;
    virtual string GetDescription() = 0;
};

// 2. Các chiến lược cụ thể
class NoDiscount : public IPromotionStrategy {
public:
    double CalculateDiscount(double total) override { return 0; }
    string GetDescription() override { return "Khong giam"; }
};

class VipDiscount : public IPromotionStrategy {
public:
    double CalculateDiscount(double total) override { return total * 0.1; } // Giảm 10%
    string GetDescription() override { return "Giam 10% VIP"; }
};

class GoldenHourDiscount : public IPromotionStrategy {
public:
    double CalculateDiscount(double total) override { 
        return (total > 100) ? 20 : 0; // Giảm 20k nếu > 100k
    }
    string GetDescription() override { return "Giam 20k Gio vang"; }
};

// 3. Context (Hóa đơn) - Sử dụng Strategy
class Bill : public BaseObject, public CompositeNode {
private:
    IPromotionStrategy* _strategy; // Chứa chiến lược hiện tại
public:
    Bill(string name) : BaseObject(name) {
        _strategy = new NoDiscount(); // Mặc định không giảm
    }

    void SetStrategy(IPromotionStrategy* s) {
        if (_strategy) delete _strategy; // Xóa cũ
        _strategy = s;
    }

    // Tính tổng tiền các món (Chưa giảm)
    double GetSubTotal() const {
        double total = 0;
        for (auto item : *this) total += item->GetValue();
        return total;
    }

    // Tính tiền cuối cùng (Đã trừ khuyến mãi)
    double GetValue() const override {
        double sub = GetSubTotal();
        double discount = _strategy->CalculateDiscount(sub);
        return sub - discount;
    }

    void Xuat(ostream& os) const override {
        os << "[BILL] " << _name << "\n";
        for (auto item : *this) {
            os << "  - "; item->Xuat(os); 
            os << " : " << item->GetValue() << "k\n";
        }
        os << "  => Tam tinh: " << GetSubTotal() << "k\n";
        os << "  => Khuyen mai (" << _strategy->GetDescription() << "): -" 
           << _strategy->CalculateDiscount(GetSubTotal()) << "k\n";
        os << "  => THANH TIEN: " << GetValue() << "k";
    }
};

// =========================================================================
// HÀM MAIN
// =========================================================================
/*
int main() {
    // 1. Tạo đơn hàng
    Bill* bill = new Bill("Ban so 1");

    // 2. Tạo món: Cafe Sữa Đá (Espresso + Sữa + Đá)
    // Decorator lồng nhau: Da(Sua(Espresso))
    BaseObject* ly1 = new Coffee("Espresso", 30);
    ly1 = new ToppingDecorator(ly1, "Sua", 5);
    ly1 = new ToppingDecorator(ly1, "Da", 2);
    
    // Tạo món: HouseBlend + Trân châu
    BaseObject* ly2 = new Coffee("HouseBlend", 20);
    ly2 = new ToppingDecorator(ly2, "Tran Chau", 10);

    bill->Add(ly1);
    bill->Add(ly2);

    // 3. Tính tiền (Mặc định không giảm)
    cout << *bill << endl << endl;

    // 4. Khách đưa thẻ VIP -> Đổi chiến lược
    bill->SetStrategy(new VipDiscount());
    cout << "--- SAU KHI QUET THE VIP ---" << endl;
    cout << *bill << endl;

    return 0;
}
*/
