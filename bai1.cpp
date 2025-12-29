#include "AllInOne.h"

// ====================================================
// 1. SINGLETON: NGUỒN ĐIỆN (POWER SUPPLY)
// Nhiệm vụ: Lưu U, I_max và kiểm tra an toàn điện
// ====================================================
class PowerSupply : public Singleton<PowerSupply> {
    friend class Singleton<PowerSupply>;
    double U;    // Hiệu điện thế (Volt)
    double I_max; // Cường độ tối đa (Ampe)
    PowerSupply() : U(220), I_max(10) {} // Mặc định

public:
    void SetVoltage(double v) { U = v; }
    void SetMaxCurrent(double i) { I_max = i; }

    // Hàm kiểm tra an toàn: Tính I = U / R
    void CheckSafety(double R) {
        if (R <= 0) throw AppError("Doan mach (R=0) hoac R am!");
        
        double I = U / R;
        if (I > I_max) {
            throw AppError("QUA TAI! I = " + to_string((int)I) + "A > Max " + to_string((int)I_max) + "A");
        }
    }
    
    double GetVoltage() const { return U; }
};

// ====================================================
// 2. LEAF: ĐIỆN TRỞ (RESISTOR)
// ====================================================
class Resistor : public BaseObject {
private:
    double _ohm;
public:
    // Constructor
    Resistor(string name, double ohm) : BaseObject(name), _ohm(ohm) {}

    // Override lấy giá trị điện trở
    double GetValue() const override { return _ohm; }

    void Xuat(ostream& os) const override {
        os << "[R] " << _name << " (" << _ohm << " Ohm)";
    }
};

// ====================================================
// 3. COMPOSITE 1: MẠCH NỐI TIẾP (SERIAL CIRCUIT)
// Logic: R_td = R1 + R2 + ...
// ====================================================
class SerialCircuit : public BaseObject, public CompositeNode {
public:
    SerialCircuit(string n) : BaseObject(n) {}

    double GetValue() const override {
        double sum = 0;
        // Duyệt qua các phần tử con (nhờ Iterator trong AllInOne.h)
        for (auto child : *this) {
            sum += child->GetValue();
        }
        return sum;
    }

    // Override Add: Mạch nối tiếp càng thêm R càng lớn -> Luôn an toàn về I
    // Nên có thể không cần check, hoặc check cho đúng quy trình.
    void Add(BaseObject* obj) override {
        if (obj) CompositeNode::Add(obj);
    }

    // XỬ LÝ TOÁN TỬ DỊ: *mainBoard = *mainBoard + 20;
    // Ý nghĩa: Nối tiếp thêm 1 điện trở 20 Ohm vào mạch
    SerialCircuit& operator+(int ohm) {
        // Tạo một điện trở mới và Add vào chính mình
        string name = "R_Added_" + to_string(ohm);
        this->Add(new Resistor(name, (double)ohm));
        return *this; // Trả về tham chiếu để gán lại được
    }

    void Xuat(ostream& os) const override {
        os << "[NOI TIEP] " << _name << " { R_tong: " << GetValue() << " }";
        for (auto c : *this) os << "\n    |-- " << *c;
    }
};

// ====================================================
// 4. COMPOSITE 2: MẠCH SONG SONG (PARALLEL CIRCUIT)
// Logic: 1/R_td = 1/R1 + 1/R2 + ...
// BẪY LỚN: Mạch song song càng thêm R thì R_td càng NHỎ -> Dễ quá tải I
// ====================================================
class ParallelCircuit : public BaseObject, public CompositeNode {
public:
    ParallelCircuit(string n) : BaseObject(n) {}

    // Logic tính toán khó nhất bài
    double GetValue() const override {
        double invTotal = 0; // Tổng nghịch đảo
        if (begin() == end()) return 0; // Chưa có linh kiện

        for (auto child : *this) {
            double r = child->GetValue();
            if (r > 0) invTotal += (1.0 / r);
        }
        
        if (invTotal == 0) return 0;
        return 1.0 / invTotal; // Nghịch đảo lại để ra R tương đương
    }

    // Override Add: Mạch song song thêm R sẽ làm giảm R tổng -> Cần Check
    void Add(BaseObject* obj) override {
        if (obj) {
            // Thêm tạm vào để tính R mới
            CompositeNode::Add(obj);
            
            // Tính R tổng mới của cụm này
            double newR = this->GetValue();

            // Gọi Singleton kiểm tra: Với U hiện tại, R này có làm cháy mạch không?
            // (Lưu ý: Đây là check cục bộ, giả sử cụm này chịu toàn bộ U)
            PowerSupply::GetInstance()->CheckSafety(newR);
        }
    }

    void Xuat(ostream& os) const override {
        os << "[SONG SONG] " << _name << " { R_td: " << GetValue() << " }";
        for (auto c : *this) os << "\n    |== " << *c;
    }
};

// ====================================================
// 5. FACTORY: XỬ LÝ CHUỖI "R1 - 100"
// ====================================================
class ResistorFactory {
public:
    static BaseObject* Create(string s) {
        // Bước 1: Thay thế ký tự lạ '-' thành ',' để dùng Utils::Split
        // (Đây là kỹ thuật Adaptations.h)
        for (char &c : s) {
            if (c == '-') c = ',';
        }

        // Bước 2: Cắt chuỗi
        vector<string> parts = Utils::Split(s, ',');
        
        if (parts.size() >= 2) {
            string name = parts[0];
            double ohm = Utils::ToNum(parts[1]);
            return new Resistor(name, ohm);
        }
        return nullptr;
    }
};
