#include "AllInOne.h"

// ========================================================================
// BƯỚC 1: XÂY DỰNG SINGLETON (ENERGY MONITOR)
// Tư duy:
// - Giống bài Xe tải, ta cần quản lý giới hạn tổng công suất (Max Power).
// - Nếu thêm thiết bị làm vượt quá công suất chịu tải của đường dây -> Báo lỗi.
// ========================================================================

class EnergyMonitor : public Singleton<EnergyMonitor> {
    friend class Singleton<EnergyMonitor>;
    double maxWatt;
    double currentWatt;

    EnergyMonitor() : maxWatt(0), currentWatt(0) {}

public:
    void SetLimit(double w) { maxWatt = w; }

    // Check xem thêm thiết bị mới có bị quá tải hệ thống không
    void CheckLoad(double watt) {
        if (currentWatt + watt > maxWatt) {
            throw AppError("QUA TAI DIEN! Gioi han: " + to_string((int)maxWatt) + 
                           "W. Hien tai: " + to_string((int)currentWatt) + 
                           "W. Them: " + to_string((int)watt) + "W.");
        }
        currentWatt += watt;
    }
    
    // Hàm giảm tải (khi xóa thiết bị hoặc tắt bớt - nếu đề yêu cầu logic phức tạp hơn)
    // Ở bài này ta chỉ check khi Add (giả sử công suất đăng ký tối đa)
};

// ========================================================================
// BƯỚC 2: CLASS TRUNG GIAN (SMART UNIT)
// Tư duy:
// - BaseObject không có biến trạng thái (_isOn).
// - Ta cần tạo SmartUnit kế thừa BaseObject để thêm thuộc tính này.
// - Đây là nơi định nghĩa logic "Lan truyền": SetState().
// ========================================================================

class SmartUnit : public BaseObject {
protected:
    bool _isOn; // Trạng thái Bật/Tắt
public:
    SmartUnit(string name) : BaseObject(name), _isOn(true) {} // Mặc định bật

    // Hàm ảo để lan truyền trạng thái (Composite sẽ override hàm này)
    virtual void SetState(bool state) {
        _isOn = state;
    }

    bool IsOn() const { return _isOn; }
};

// ========================================================================
// BƯỚC 3: CLASS LÁ - THIẾT BỊ (DEVICE)
// Tư duy:
// - Thiết bị có công suất cố định (_power).
// - GetValue() (Công suất tiêu thụ thực tế) phụ thuộc vào _isOn.
//   + Nếu ON -> Trả về _power.
//   + Nếu OFF -> Trả về 0.
// ========================================================================

class Device : public SmartUnit {
private:
    double _power; // Công suất danh định
    string _type;  // Loại thiết bị (Light, Fridge...)
public:
    Device(string type, string name, double power) 
        : SmartUnit(name), _type(type), _power(power) {}

    // Logic quan trọng: Tắt là không tốn điện
    double GetValue() const override {
        return _isOn ? _power : 0;
    }

    // Override SetState (Leaf chỉ tự set chính nó)
    void SetState(bool state) override {
        _isOn = state;
    }

    void Xuat(ostream& os) const override {
        os << "[" << _type << "] " << _name 
           << " (" << _power << "W) - " << (_isOn ? "ON" : "OFF");
    }

    // Operator ++: Tăng công suất lên 10% (Logic dị của đề)
    Device& operator++() {
        _power *= 1.1; 
        return *this;
    }
};

// ========================================================================
// BƯỚC 4: CLASS COMPOSITE - NHÓM THIẾT BỊ / PHÒNG (SMART GROUP)
// Tư duy:
// - Kế thừa SmartUnit (để có _isOn) và CompositeNode (để có list con).
// - Logic Lan Truyền: Khi SetState cho Group -> Phải gọi SetState cho toàn bộ con.
// - Logic GetValue: Tổng công suất của các con (lưu ý con đã tự xử lý logic 0W nếu tắt).
// ========================================================================

class SmartGroup : public SmartUnit, public CompositeNode {
public:
    SmartGroup(string name) : SmartUnit(name) {}

    // 1. Logic LAN TRUYỀN TRẠNG THÁI (State Propagation)
    // Khi tắt Phòng, tất cả đèn/quạt trong phòng phải tắt theo.
    void SetState(bool state) override {
        // Set trạng thái của chính Group
        _isOn = state;

        // Duyệt qua danh sách con để Set trạng thái cho chúng
        for (auto child : *this) {
            // Cần ép kiểu vì list lưu BaseObject*, mà BaseObject không có SetState
            SmartUnit* unit = dynamic_cast<SmartUnit*>(child);
            if (unit) {
                unit->SetState(state); // Đệ quy: Nếu con là Group, nó sẽ gọi tiếp cho cháu
            }
        }
    }

    // 2. Tính Tổng Công Suất (Đệ quy)
    double GetValue() const override {
        double total = 0;
        for (auto child : *this) {
            total += child->GetValue(); // BaseObject::GetValue() đã đa hình
        }
        return total;
    }

    // 3. Add thiết bị có check quá tải
    void Add(BaseObject* obj) override {
        if (obj) {
            // Check công suất danh định (khi bật)
            // Lưu ý: Có thể đề yêu cầu check công suất tối đa (lúc bật)
            // Nên ta tạm bật nó lên để lấy Max Power check, hoặc giả định GetValue lúc này là max
            EnergyMonitor::GetInstance()->CheckLoad(obj->GetValue());
            CompositeNode::Add(obj);
        }
    }

    void Xuat(ostream& os) const override {
        os << "[GROUP] " << _name 
           << " (Total: " << GetValue() << "W) - " 
           << (_isOn ? "ON" : "OFF");
           
        for (auto c : *this) {
            os << "\n    + " << *c;
        }
    }
};

// ========================================================================
// BƯỚC 5: FACTORY - PARSING CHUỖI PHỨC TẠP
// Tư duy:
// - Input: "[Light] Den Tran (100)"
// - Ký tự gây nhiễu: [ ] ( )
// - Chiến thuật: Thay thế tất cả ký tự nhiễu thành dấu phẩy ',' rồi Split.
// -> "[Light] Den Tran (100)" -> ",Light, Den Tran ,100,"
// -> Split sẽ lọc bỏ rỗng, ta thu được: ["Light", "Den Tran", "100"]
// ========================================================================

class DeviceFactory {
public:
    static BaseObject* Create(string s) {
        // 1. Làm sạch chuỗi (Thay ký tự lạ bằng phẩy)
        for (char &c : s) {
            if (c == '[' || c == ']' || c == '(' || c == ')') c = ',';
        }

        // 2. Cắt chuỗi
        vector<string> parts = Utils::Split(s, ',');
        
        // Mong đợi: [Type, Name, Power]
        if (parts.size() >= 3) {
            string type = parts[0];
            string name = parts[1];
            double power = Utils::ToNum(parts[2]);
            return new Device(type, name, power);
        }
        return nullptr;
    }
};

Hướng dẫn kiểm tra lại (Mental Check)
Vấn đề ép kiểu (Casting):

Trong SmartGroup::SetState, mình dùng dynamic_cast<SmartUnit*>(child).

// Tại sao? Vì CompositeNode lưu vector<BaseObject*>. BaseObject không hề có hàm SetState. Chỉ có SmartUnit (lớp con) mới có. Nếu không ép kiểu, code sẽ lỗi biên dịch.

// Logic GetValue của Device:

// return _isOn ? _power : 0;

// Dòng này cực kỳ quan trọng. Nếu Group tắt (SetState(false)), nó sẽ set _isOn = false. Khi đó dù _power vẫn là 100W, nhưng hàm GetValue trả về 0W -> Đúng thực tế.

// Parsing chuỗi:

// Kỹ thuật replace thành dấu phẩy là kỹ thuật "tà đạo" nhưng cực kỳ hiệu quả trong bài thi giới hạn thời gian. Bạn không cần regex hay xử lý chuỗi phức tạp.

// MAIN.CPP
#include "Solutions.cpp"

int main() {
    try {
        // 1. Singleton: EnergyMonitor (Giám sát điện)
        // Giới hạn 5000W
        EnergyMonitor::GetInstance()->SetLimit(5000);

        // 2. Composite: Room (Phòng), House (Nhà)
        SmartGroup* house = new SmartGroup("My House");
        SmartGroup* kitchen = new SmartGroup("Kitchen");

        // 3. Leaf: Device (ThietBi)
        // Format: "[Light] Den Tran (100)" -> Dùng ngoặc vuông, ngoặc tròn
        // BẪY: Parsing chuỗi cực dị.
        Device* d1 = DeviceFactory::Create("[Fridge] Tu Lanh (200)");
        Device* d2 = DeviceFactory::Create("[Light] Den Bep (50)");

        kitchen->Add(d1);
        kitchen->Add(d2);
        house->Add(kitchen);

        // 4. LOGIC LAN TRUYỀN (State Propagation)
        // Yêu cầu: Khi tắt Nhà, Bếp phải tắt -> Đèn & Tủ lạnh phải tắt theo.
        // Thiết bị tắt thì công suất tiêu thụ = 0.
        
        cout << "--- BAT TOAN BO ---" << endl;
        house->SetState(true); // ON
        cout << "Tong cong suat: " << house->GetValue() << "W" << endl; // Phải là 250W

        cout << "--- TAT TOAN BO ---" << endl;
        house->SetState(false); // OFF
        cout << "Tong cong suat: " << house->GetValue() << "W" << endl; // Phải là 0W (Dù size vẫn set là 200, 50)

        // 5. Operator ++: Tăng công suất thiết bị lên 10%
        // (++(*d1)); 
        
    } catch (exception& e) {
        cout << "LOI SMART HOME: " << e.what() << endl;
    }
    return 0;
}
