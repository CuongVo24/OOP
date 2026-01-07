#pragma once // Ngăn việc include file này nhiều lần gây lỗi
#include <string>
#include <vector>
#include <iostream>

using namespace std;

// ==========================================
// 1. ABSTRACT BASE CLASS (Lớp Trừu Tượng)
// ==========================================
// Đây là "Interface" quy định mọi luật đổi tên phải tuân theo.
// Tính chất: TRỪU TƯỢNG (Abstraction) & ĐA HÌNH (Polymorphism)
class IRule {
public:
    // Hàm thuần ảo (Pure Virtual Function): Bắt buộc lớp con phải viết code cho hàm này.
    // Input: Tên file cũ. Output: Tên file mới sau khi áp dụng luật.
    virtual string rename(const string& filename) = 0;

    // Virtual Destructor: CỰC KỲ QUAN TRỌNG.
    // Giúp xóa đúng đối tượng con khi delete qua con trỏ cha.
    virtual ~IRule() = default;
};

// ==========================================
// 2. CONCRETE CLASSES (Các Luật Cụ Thể)
// ==========================================

// Luật 1: Chuẩn hóa viết hoa/thường
class NormalizeCasing : public IRule {
public:
    string rename(const string& filename) override;
};

// Luật 2: Xóa ký tự đặc biệt
class RemoveSpecialCharacters : public IRule {
private:
    // Có thể lưu danh sách ký tự cấm nếu muốn mở rộng sau này
    // vector<char> specialChars; 
public:
    string rename(const string& filename) override;
};

// Luật 3: Thêm tiền tố ngày tháng
class AddDatePrefix : public IRule {
public:
    string rename(const string& filename) override;
};

// ==========================================
// 3. FACTORY CLASS (Nhà máy sản xuất luật)
// ==========================================
// Nhiệm vụ: Đọc một chuỗi (ví dụ từ file rules.txt) và tạo ra đối tượng Rule tương ứng.
// Giúp tách biệt việc "tạo đối tượng" khỏi "sử dụng đối tượng".
class RuleFactory {
public:
    // Static method: Gọi trực tiếp không cần tạo đối tượng Factory
    // Input: Dòng text cấu hình (VD: "NormalizeCasing")
    // Output: Con trỏ đến luật tương ứng
    static IRule* createRule(const string& ruleType);
};

// ==========================================
// 4. CONTEXT CLASS (Lớp Quản Lý Chính)
// ==========================================
// Nhiệm vụ: Quản lý danh sách các luật và thực thi chúng tuần tự.
class RenameCVFileUseCase {
private:
    vector<IRule*> rules; // Chứa danh sách các luật sẽ áp dụng

public:
    // Thêm một luật vào danh sách xử lý
    void addRule(IRule* rule);

    // Xử lý đổi tên: Chạy tên file qua TẤT CẢ các luật trong danh sách
    string rename(string filename);
    
    // Destructor để giải phóng bộ nhớ cho các con trỏ trong vector rules
    ~RenameCVFileUseCase();
};
