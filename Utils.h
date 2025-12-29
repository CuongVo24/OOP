#ifndef UTILS_H
#define UTILS_H

#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <fstream>
#include <algorithm>
#include "Core.h" // Kết nối với Core để dùng MyException

using namespace std;

// ================================================================
// MODULE 5: STRING HELPER (Xử lý chuỗi - Cực quan trọng cho File I/O)
// ----------------------------------------------------------------
// Chức năng: Cắt chuỗi, xóa khoảng trắng thừa, chuyển đổi số.
// Dùng cho: Các file có định dạng phức tạp (CSV, Log file).
// ================================================================
class StringHelper {
public:
    // 1. Hàm Trim: Xóa khoảng trắng thừa ở đầu và cuối chuỗi
    // VD: "  Nguyen Van A  " -> "Nguyen Van A"
    static string Trim(const string& str) {
        string s = str;
        // Xóa đầu
        s.erase(s.begin(), find_if(s.begin(), s.end(), [](unsigned char ch) {
            return !isspace(ch);
        }));
        // Xóa đuôi
        s.erase(find_if(s.rbegin(), s.rend(), [](unsigned char ch) {
            return !isspace(ch);
        }).base(), s.end());
        return s;
    }

    // 2. Hàm Split: Cắt chuỗi theo ký tự phân cách
    // VD: "NV01, Nguyen A, 100" -> vector gồm ["NV01", "Nguyen A", "100"]
    static vector<string> Split(string str, char delimiter) {
        vector<string> tokens;
        string token;
        stringstream ss(str);
        
        while (getline(ss, token, delimiter)) {
            // Trim ngay sau khi cắt để đảm bảo sạch dữ liệu
            tokens.push_back(Trim(token));
        }
        return tokens;
    }

    // 3. Hàm ToInt / ToDouble: Chuyển đổi an toàn
    static int ToInt(string str) {
        try {
            return stoi(str);
        } catch (...) { return 0; } // Trả về 0 nếu lỗi
    }

    static double ToDouble(string str) {
        try {
            return stod(str);
        } catch (...) { return 0.0; }
    }
};

// ================================================================
// MODULE 6: FILE HELPER (Hỗ trợ mở file an toàn)
// ----------------------------------------------------------------
// Chức năng: Mở file và tự ném ngoại lệ nếu không tìm thấy.
// Giúp code trong main gọn gàng, không phải if-else kiểm tra file.
// ================================================================
class FileHelper {
public:
    static ifstream OpenFile(string filename) {
        ifstream f(filename);
        if (!f.is_open()) {
            // Ném exception từ Core.h
            throw MyException("Khong the mo file: " + filename);
        }
        return f;
    }
};


#include "Core.h"
#include "Utils.h"

// Giả sử đang ở trong hàm Nhap của class NhanVien
// Hoặc đang xử lý trong Main
void DocFileNhanVien(string filename) {
    try {
        // 1. Mở file an toàn (Module 6)
        ifstream f = FileHelper::OpenFile(filename);
        
        string line;
        while (getline(f, line)) {
            // Bỏ qua dòng trống
            if (line.empty()) continue; 

            // 2. Cắt chuỗi (Module 5)
            // line = "NV01, Nguyen Van A, 15/05/1990, 5000000"
            vector<string> data = StringHelper::Split(line, ',');
            
            // data[0] = "NV01"
            // data[1] = "Nguyen Van A" (đã tự động Trim sạch khoảng trắng)
            // data[2] = "15/05/1990"
            // data[3] = "5000000"

            // 3. Sử dụng dữ liệu
            string ma = data[0];
            string ten = data[1];
            
            // Chuyển đổi chuỗi ngày tháng sang object Date (cần xử lý thêm chút ở operator >> của Date hoặc parse tay)
            // Ở đây ví dụ lấy lương:
            double luong = StringHelper::ToDouble(data[3]); 
            
            cout << "Da doc: " << ten << " - Luong: " << luong << endl;
        }
        f.close();
    }
    catch (exception& e) {
        cout << "Loi doc file: " << e.what() << endl;
    }
}
#endif // UTILS_H
