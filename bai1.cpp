#include "AllInOne.h"

// =========================================================================
// KHU VỰC 1: SINGLETON - QUẢN LÝ NGUỒN (POWER SUPPLY)
// [MAP TO MAIN]: PowerSupply::GetInstance()->SetVoltage(220.0);
// [TƯ DUY]: Quản lý thông số toàn cục (U, I_max) và kiểm tra an toàn hệ thống.
// =========================================================================
class PowerSupply : public Singleton<PowerSupply> {
    friend class Singleton<PowerSupply>;
    double _U;    // Hiệu điện thế (V)
    double _Imax; // Cường độ tối đa (A)
    PowerSupply() : _U(0), _Imax(0) {}

public:
    void SetVoltage(double v) { _U = v; }
    void SetMaxCurrent(double i) { _Imax = i; }

    // [MAP TO MAIN]: Check Singleton logic khi Add linh kiện
    // [LOGIC]: I = U / R. Nếu I > Imax -> Cháy mạch.
    void CheckSafety(double R_td) {
        if (R_td <= 0) throw AppError("LOI: Doan mach hoac R am!");
        
        double I = _U / R_td;
        if (I > _Imax) {
            throw AppError("QUA TAI! I=" + to_string((int)I) + "A > Max " + to_string((int)_Imax) + "A");
        }
    }
};

// =========================================================================
// KHU VỰC 2: CLASS LÁ - ĐIỆN TRỞ (RESISTOR)
// [MAP TO MAIN]: ResistorFactory::Create("R1 - 100");
// =========================================================================
class Resistor : public BaseObject {
private:
    double _ohm;
public:
    // [MACRO]: Tạo Constructor nhanh (BaseObject đã lo phần _name)
    DECLARE_CTOR(Resistor, _ohm)

    // [MAP TO MAIN]: Tính toán giá trị -> Trả về số Ohm
    double GetValue() const override { return _ohm; }

    void Xuat(ostream& os) const override {
        os << "[R] " << _name << " (" << _ohm << " Ohm)";
    }
};

// =========================================================================
// KHU VỰC 3: COMPOSITE 1 - MẠCH NỐI TIẾP (SERIAL CIRCUIT)
// [MAP TO MAIN]: Logic R = R1 + R2 + ...
// =========================================================================
class SerialCircuit : public BaseObject, public CompositeNode {
public:
    SerialCircuit(string n) : BaseObject(n) {}

    // [LOGIC TOÁN HỌC]: Cộng dồn các con
    double GetValue() const override {
        double sum = 0;
        for (auto child : *this) sum += child->GetValue();
        return sum;
    }

    // [MAP TO MAIN]: *mainBoard = *mainBoard + 20;
    // [TƯ DUY]: Toán tử + tạo ra một điện trở ảo rồi Add vào mạch
    SerialCircuit& operator+(int ohm) {
        string name = "R_ao_" + to_string(ohm);
        this->Add(new Resistor(name, (double)ohm));
        return *this; 
    }
    
    // [MAP TO MAIN]: In ra sơ đồ
    void Xuat(ostream& os) const override {
        os << "[NOI TIEP] " << _name << " (R_tong: " << GetValue() << ")";
        for (auto c : *this) os << "\n    |-- " << *c;
    }
};

// =========================================================================
// KHU VỰC 4: COMPOSITE 2 - MẠCH SONG SONG (PARALLEL CIRCUIT)
// [MAP TO MAIN]: Logic 1/R = 1/R1 + 1/R2 ...
// [BẪY]: Thêm R vào song song làm R_tong giảm -> I tăng -> Dễ cháy.
// =========================================================================
class ParallelCircuit : public BaseObject, public CompositeNode {
public:
    ParallelCircuit(string n) : BaseObject(n) {}

    // [LOGIC TOÁN HỌC]: Tính tổng nghịch đảo
    double GetValue() const override {
        double invTotal = 0;
        if (begin() == end()) return 0; // Chưa có linh kiện
        
        for (auto child : *this) {
            double r = child->GetValue();
            if (r > 0) invTotal += (1.0 / r);
        }
        return (invTotal == 0) ? 0 : (1.0 / invTotal);
    }

    // [MAP TO MAIN]: Kiểm tra an toàn điện khi Add
    // Vì R song song càng thêm càng nhỏ -> Dễ gây quá tải
    void Add(BaseObject* obj) override {
        if (obj) {
            CompositeNode::Add(obj); // Thêm trước để tính R mới
            
            // Gọi Singleton check ngay lập tức
            double newR = this->GetValue(); 
            PowerSupply::GetInstance()->CheckSafety(newR);
        }
    }

    void Xuat(ostream& os) const override {
        os << "[SONG SONG] " << _name << " (R_td: " << GetValue() << ")";
        for (auto c : *this) os << "\n    |== " << *c;
    }
};

// =========================================================================
// KHU VỰC 5: FACTORY - XỬ LÝ CHUỖI DỊ
// [MAP TO MAIN]: Create("R1 - 100") -> Dấu gạch ngang
// =========================================================================
class ComponentFactory {
public:
    static BaseObject* Create(string s) {
        // [XỬ LÝ CHUỖI]: Thay '-' thành ',' để dùng được Utils::Split
        replace(s.begin(), s.end(), '-', ',');
        
        vector<string> p = Utils::Split(s, ',');
        if (p.size() >= 2) {
            return new Resistor(p[0], Utils::ToNum(p[1]));
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
        cout << "--- TEST MACH DIEN ---" << endl;
        
        // 1. Setup Singleton
        PowerSupply::GetInstance()->SetVoltage(220.0);
        PowerSupply::GetInstance()->SetMaxCurrent(10.0);

        // 2. Factory tạo linh kiện (Lưu ý kiểu trả về là BaseObject*)
        BaseObject* r1 = ComponentFactory::Create("R1 - 100");
        BaseObject* r2 = ComponentFactory::Create("R2 - 100");
        BaseObject* r3 = ComponentFactory::Create("R3 - 50");

        // 3. Mạch Song Song (Composite)
        ParallelCircuit* paraGroup = new ParallelCircuit("Cum Song Song");
        paraGroup->Add(r1);
        paraGroup->Add(r2); // R_td = 50 Ohm

        // 4. Mạch Nối Tiếp (Composite)
        SerialCircuit* mainBoard = new SerialCircuit("Mach Chinh");
        mainBoard->Add(paraGroup);
        mainBoard->Add(r3); // R_tong = 50 + 50 = 100 Ohm

        // 5. Operator +: Thêm điện trở ảo 20 Ohm
        *mainBoard = *mainBoard + 20; // R_tong = 120 Ohm

        // 6. Xuất kết quả
        cout << *mainBoard << endl; 
        
        // 7. Test Check Singleton (Quá tải)
        // R_ngan_mach = 1 Ohm -> R_song_song ~ 0 -> I cực lớn -> Exception
        cout << "\n>> THU LAM DOAN MACH..." << endl;
        paraGroup->Add(ComponentFactory::Create("R_Short - 1")); 

    } catch (exception& e) {
        cout << "[SU CO]: " << e.what() << endl;
    }
    return 0;
}
*/
