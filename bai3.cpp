#include "AllInOne.h"

// =========================================================================
// KHU VỰC 1: SINGLETON - GIÁM SÁT ĐIỆN (ENERGY MONITOR)
// [MAP TO MAIN]: EnergyMonitor::GetInstance()->SetLimit(5000);
// [TƯ DUY]: Quản lý giới hạn công suất toàn hệ thống.
// =========================================================================
class EnergyMonitor : public Singleton<EnergyMonitor> {
    friend class Singleton<EnergyMonitor>;
    double _maxWatt;
    double _currentWatt;

    EnergyMonitor() : _maxWatt(0), _currentWatt(0) {}

public:
    void SetLimit(double w) { _maxWatt = w; }

    // [MAP TO MAIN]: Check quá tải khi thêm thiết bị
    void CheckLoad(double watt) {
        if (_currentWatt + watt > _maxWatt) {
            throw AppError("QUA TAI DIEN! Max: " + to_string((int)_maxWatt) + 
                           "W. Hien tai: " + to_string((int)_currentWatt) + "W.");
        }
        _currentWatt += watt;
    }
};

// =========================================================================
// KHU VỰC 2: INTERFACE TRUNG GIAN (SMART UNIT)
// [TƯ DUY]: Tại sao cần class này?
// - BaseObject không có biến trạng thái Bật/Tắt (_isOn).
// - Ta cần class này để định nghĩa hàm ảo SetState() cho cả Device và Group.
// =========================================================================
class SmartUnit : public BaseObject {
protected:
    bool _isOn; // Trạng thái: true = ON, false = OFF
public:
    SmartUnit(string name) : BaseObject(name), _isOn(true) {} // Mặc định ON

    // Hàm ảo để lan truyền trạng thái (Composite sẽ override hàm này để gọi cho con)
    virtual void SetState(bool state) {
        _isOn = state;
    }
};

// =========================================================================
// KHU VỰC 3: CLASS LÁ - THIẾT BỊ (DEVICE)
// [MAP TO MAIN]: DeviceFactory::Create(...)
// =========================================================================
class Device : public SmartUnit {
private:
    double _power; // Công suất danh định
    string _type;  // Loại (Light, Fridge...)
public:
    Device(string type, string name, double power) 
        : SmartUnit(name), _type(type), _power(power) {}

    // [LOGIC QUAN TRỌNG]: Tắt là không tốn điện (0W)
    double GetValue() const override {
        return _isOn ? _power : 0;
    }

    void Xuat(ostream& os) const override {
        os << "[" << _type << "] " << _name 
           << " (" << _power << "W) - " << (_isOn ? "ON" : "OFF");
    }

    // [MAP TO MAIN]: (++(*d1)) -> Tăng công suất 10%
    Device& operator++() {
        _power *= 1.1; 
        return *this;
    }
};

// =========================================================================
// KHU VỰC 4: COMPOSITE - PHÒNG / NHÓM (SMART GROUP)
// [MAP TO MAIN]: house->SetState(false);
// [TƯ DUY]: Kế thừa SmartUnit (để có _isOn) và CompositeNode (để chứa con).
// =========================================================================
class SmartGroup : public SmartUnit, public CompositeNode {
public:
    SmartGroup(string name) : SmartUnit(name) {}

    // [LOGIC LAN TRUYỀN]: Khi Cha tắt -> Ép buộc tất cả Con tắt theo
    void SetState(bool state) override {
        // 1. Set chính mình
        _isOn = state;

        // 2. Set cho đàn con
        for (auto child : *this) {
            // [BẪY ÉP KIỂU]: List con là BaseObject*, phải ép sang SmartUnit* mới có SetState
            SmartUnit* unit = dynamic_cast<SmartUnit*>(child);
            if (unit) {
                unit->SetState(state); // Đệ quy: Nếu con là Group, nó gọi tiếp cho cháu
            }
        }
    }

    // [LOGIC TÍNH TỔNG]: Cộng dồn công suất thực tế (đã tính vụ tắt/mở ở bước trên)
    double GetValue() const override {
        double total = 0;
        for (auto child : *this) total += child->GetValue();
        return total;
    }

    // [MAP TO MAIN]: Add thiết bị có check quá tải
    void Add(BaseObject* obj) override {
        if (obj) {
            // Check công suất tối đa (giả định bật) với Singleton
            EnergyMonitor::GetInstance()->CheckLoad(obj->GetValue());
            CompositeNode::Add(obj);
        }
    }

    void Xuat(ostream& os) const override {
        os << "[GROUP] " << _name 
           << " (Total: " << GetValue() << "W) - " 
           << (_isOn ? "ON" : "OFF");
        for (auto c : *this) os << "\n    + " << *c;
    }
};

// =========================================================================
// KHU VỰC 5: FACTORY - XỬ LÝ CHUỖI DỊ
// [MAP TO MAIN]: Create("[Light] Den Tran (100)")
// [TƯ DUY]: Có các ký tự nhiễu [ ] ( ) -> Thay hết bằng dấu phẩy
// =========================================================================
class DeviceFactory {
public:
    static BaseObject* Create(string s) {
        // [XỬ LÝ CHUỖI]: Thay ký tự lạ bằng phẩy
        for (char &c : s) {
            if (c == '[' || c == ']' || c == '(' || c == ')') c = ',';
        }

        // Sau khi thay: ",Light, Den Tran ,100," -> Split bỏ rỗng -> Lấy được data
        vector<string> p = Utils::Split(s, ',');
        
        if (p.size() >= 3) {
            string type = p[0];
            string name = p[1];
            double power = Utils::ToNum(p[2]);
            return new Device(type, name, power);
        }
        return nullptr;
    }
};

// =========================================================================
// KHU VỰC 6: HÀM MAIN (ĐỀ BÀI)
// =========================================================================
/*
int main() {
    try {
        // 1. Singleton: Giới hạn 5000W
        EnergyMonitor::GetInstance()->SetLimit(5000);

        // 2. Composite: Nhà và Bếp
        SmartGroup* house = new SmartGroup("My House");
        SmartGroup* kitchen = new SmartGroup("Kitchen");

        // 3. Factory: Tạo thiết bị (Format dị có ngoặc)
        // [Light] Den Bep (50) -> Type: Light, Name: Den Bep, Power: 50
        Device* d1 = (Device*)DeviceFactory::Create("[Fridge] Tu Lanh (200)");
        Device* d2 = (Device*)DeviceFactory::Create("[Light] Den Bep (50)");

        kitchen->Add(d1);
        kitchen->Add(d2);
        house->Add(kitchen); // House chứa Kitchen

        // 4. TEST LAN TRUYỀN: Bật toàn bộ
        cout << "--- BAT TOAN BO ---" << endl;
        house->SetState(true); 
        cout << *house << endl; // Tổng 250W

        // 5. TEST LAN TRUYỀN: Tắt toàn bộ
        // Khi tắt House -> Kitchen tắt -> d1, d2 tắt -> GetValue trả về 0
        cout << "\n--- TAT TOAN BO ---" << endl;
        house->SetState(false); 
        cout << "Tong cong suat hien tai: " << house->GetValue() << "W" << endl; // Phải là 0

        // 6. Test Operator ++
        ++(*d1); // Tăng công suất Tủ lạnh lên 10%

    } catch (exception& e) {
        cout << "LOI SMART HOME: " << e.what() << endl;
    }
    return 0;
}
*/
