#ifndef ADAPTATIONS_H
#define ADAPTATIONS_H

#include "AllInOne.h"
#include <limits> // Để dùng số nhỏ nhất/lớn nhất cho bài tìm Max/Min
#include <set>    // Để kiểm tra trùng lặp ID

// =========================================================================
// PHẦN 1: CÁC BIẾN THỂ CỦA HÀM GetValue() (LOGIC TÍNH TOÁN)
// =========================================================================

/* DẠNG 1: TÌM GIÁ TRỊ LỚN NHẤT (MAX)
   Ví dụ: Tìm lương cao nhất trong công ty, File lớn nhất trong thư mục.
   Cách dùng: Copy vào Class Composite (Folder/Department).
*/
double GetMax() const {
    double maxVal = -1; // Hoặc 0 tùy đề bài
    
    // Nếu bản thân Composite có giá trị riêng thì gán maxVal = giá trị đó
    // Nếu chỉ là vật chứa thì duyệt con:
    for (auto child : _children) {
        // Nếu con là Leaf -> so sánh trực tiếp
        // Nếu con là Composite -> Gọi đệ quy
        double val = child->GetValue(); 
        if (val > maxVal) maxVal = val;
    }
    return maxVal;
}

/* DẠNG 2: ĐẾM SỐ LƯỢNG THỎA ĐIỀU KIỆN (COUNT IF)
   Ví dụ: Đếm số nhân viên lương > 1000, Đếm số file có size > 10MB.
   Cách dùng: Copy vào Class Composite.
*/
int CountCondition(double threshold) const {
    int count = 0;
    for (auto child : _children) {
        // Cách 1: Nếu child có hàm đếm riêng -> cộng dồn
        // count += child->CountCondition(threshold); 
        
        // Cách 2: Kiểm tra trực tiếp giá trị
        if (child->GetValue() > threshold) {
            count++;
        }
    }
    return count;
}

/* DẠNG 3: TÍNH TRUNG BÌNH (AVERAGE)
   Lưu ý: Không thể lấy (avg1 + avg2)/2 được vì sai toán học.
   Phải tính: (Tổng giá trị toàn bộ) / (Tổng số phần tử).
   Cách dùng: Viết 2 hàm phụ.
*/
// Hàm 1: Tính tổng (Có sẵn GetValue rồi)
// Hàm 2: Đếm tổng số phần tử con (Lá)
int CountLeafs() const {
    int count = 0;
    for (auto child : _children) {
        // Cần cơ chế biết con là Lá hay Composite. 
        // Mẹo thi: Dynamic cast thử, hoặc mặc định BaseObject là lá trả về 1.
        // Giả sử class con override CountLeafs trả về số lượng thực tế.
        // Ở BaseObject trả về 1.
        // Ở Composite trả về tổng CountLeafs của con.
    }
    return count;
}
// -> Main sẽ gọi: obj->GetValue() / obj->CountLeafs()

/* DẠNG 4: MẠCH ĐIỆN SONG SONG (PARALLEL CIRCUIT)
   Công thức: 1/R = 1/R1 + 1/R2 + ...
   Cách dùng: Copy vào Class Composite (Mạch Song Song).
*/
double GetValueParallel() const {
    double invTotal = 0; // Tổng nghịch đảo
    if (_children.empty()) return 0;
    
    for (auto child : _children) {
        double r = child->GetValue();
        if (r > 0) invTotal += (1.0 / r);
    }
    
    if (invTotal == 0) return 0;
    return 1.0 / invTotal;
}

// =========================================================================
// PHẦN 2: CÁC BIẾN THỂ CỦA SINGLETON (LOGIC CHECK)
// =========================================================================

/* DẠNG 1: KIỂM TRA TRÙNG MÃ (UNIQUE ID CHECKER)
   Ví dụ: Không được thêm nhân viên trùng ID.
   Cách dùng: Thêm vào Singleton.
*/
class IDManager : public Singleton<IDManager> {
    friend class Singleton<IDManager>;
    set<string> existingIDs; // Dùng Set để tra cứu nhanh
    IDManager() {}
public:
    void CheckAndRegister(string id) {
        // find trả về iterator, nếu == end() tức là chưa có
        if (existingIDs.find(id) != existingIDs.end()) {
            throw AppError("Trung ma ID: " + id);
        }
        existingIDs.insert(id);
    }
};

// =========================================================================
// PHẦN 3: XỬ LÝ CHUỖI NÂNG CAO (FACTORY PARSING)
// =========================================================================

/* DẠNG: ĐỊNH DẠNG LẠ (Name - Value) HOẶC (Name (Value))
   Cách dùng: Thay thế ký tự lạ bằng dấu phẩy rồi dùng Utils::Split
*/
class AdvancedParser {
public:
    static vector<string> ParseComplexString(string s) {
        // Thay thế các ký tự gây nhiễu (- , ( , )) thành dấu phẩy
        for (char &c : s) {
            if (c == '-' || c == '(' || c == ')') c = ',';
        }
        // Giờ chuỗi "Name - Value" thành "Name , Value" -> Dùng Split cũ
        return Utils::Split(s, ',');
    }
};

#endif // ADAPTATIONS_H
