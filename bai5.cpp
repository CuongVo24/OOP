#include <iostream>
#include <string>
#include <algorithm>
#include <stdexcept>

using namespace std;

class SoNguyenLon {
private:
    string m_value;

    static SoNguyenLon s_max;
    static bool s_max_initialized;

    static string add_strings(string s1, string s2) {
        string result = "";
        int carry = 0;
        int i = s1.length() - 1;
        int j = s2.length() - 1;
        while (i >= 0 || j >= 0 || carry) {
            int sum = carry;
            if (i >= 0) sum += s1[i--] - '0';
            if (j >= 0) sum += s2[j--] - '0';
            carry = sum / 10;
            result.push_back((sum % 10) + '0');
        }
        reverse(result.begin(), result.end());
        return result;
    }

    static string subtract_strings(string s1, string s2) {
        string result = "";
        int borrow = 0;
        int i = s1.length() - 1;
        int j = s2.length() - 1;
        while (i >= 0) {
            int diff = (s1[i] - '0') - borrow - (j >= 0 ? (s2[j] - '0') : 0);
            if (diff < 0) {
                diff += 10;
                borrow = 1;
            } else borrow = 0;
            result.push_back(diff + '0');
            i--; j--;
        }
        while (result.size() > 1 && result.back() == '0') result.pop_back();
        reverse(result.begin(), result.end());
        return result;
    }

    void normalize() {
        size_t first_digit = m_value.find_first_not_of('0');
        if (first_digit == string::npos) m_value = "0";
        else m_value = m_value.substr(first_digit);

        if (m_value.length() > 100)
            throw out_of_range("Loi: So vuot qua 100 chu so.");
    }

    static void updateMax(const SoNguyenLon& num) {
        if (!s_max_initialized || num > s_max) {
            s_max = num;
            s_max_initialized = true;
        }
    }

public:
    // Constructors
    SoNguyenLon() : m_value("0") { updateMax(*this); }

    SoNguyenLon(long long n) {
        if (n < 0) throw invalid_argument("Khong ho tro so am.");
        m_value = to_string(n);
        normalize();
        updateMax(*this);
    }

    SoNguyenLon(const string& s) : m_value(s) {
        normalize();
        updateMax(*this);
    }

    SoNguyenLon(int digit, int count) {
        if (digit < 0 || digit > 9 || count <= 0) m_value = "0";
        else m_value.assign(count, '0' + digit);
        normalize();
        updateMax(*this);
    }

    // Operator Overloads
    SoNguyenLon operator+(const SoNguyenLon& other) const {
        return SoNguyenLon(add_strings(m_value, other.m_value));
    }

    SoNguyenLon operator-(const SoNguyenLon& other) const {
        if (*this < other) return SoNguyenLon("0");
        return SoNguyenLon(subtract_strings(m_value, other.m_value));
    }

    bool operator>(const SoNguyenLon& other) const {
        if (m_value.length() != other.m_value.length())
            return m_value.length() > other.m_value.length();
        return m_value > other.m_value;
    }

    bool operator<(const SoNguyenLon& other) const {
        if (m_value.length() != other.m_value.length())
            return m_value.length() < other.m_value.length();
        return m_value < other.m_value;
    }

    bool operator==(const SoNguyenLon& other) const {
        return m_value == other.m_value;
    }

    // Hỗ trợ
    friend SoNguyenLon operator+(long long lhs, const SoNguyenLon& rhs) {
        return SoNguyenLon(lhs) + rhs;
    }
    friend SoNguyenLon operator-(long long lhs, const SoNguyenLon& rhs) {
        return SoNguyenLon(lhs) - rhs;
    }
    friend SoNguyenLon operator+(const SoNguyenLon& lhs, long long rhs) {
        return lhs + SoNguyenLon(rhs);
    }
    friend SoNguyenLon operator-(const SoNguyenLon& lhs, long long rhs) {
        return lhs - SoNguyenLon(rhs);
    }

    // Xuất ra màn hình
    friend ostream& operator<<(ostream& os, const SoNguyenLon& snl) {
        os << snl.m_value;
        return os;
    }

    static SoNguyenLon SNLMax() { return s_max; }
};

// Khởi tạo biến static
SoNguyenLon SoNguyenLon::s_max("0");
bool SoNguyenLon::s_max_initialized = false;

// MAIN
int main() {
    try {
        SoNguyenLon snl1;
        SoNguyenLon snl2(1234567);
        SoNguyenLon snl3(3, 14);
        SoNguyenLon snl4(9, 20);

        SoNguyenLon snl5 = snl3 - snl2;
        SoNguyenLon snl6 = 45678910 - snl2;
        SoNguyenLon snl7 = snl4 - snl3 + 123456789;

        cout << snl1 << endl << snl2 << endl << snl3 << endl;
        cout << snl4 << endl << snl5 << endl << snl6 << endl;
        cout << snl7 << endl << endl;

        cout << "SoNguyenLon Max:" << endl;
        cout << SoNguyenLon::SNLMax() << endl;
    }
    catch (const exception& e) {
        cout << e.what() << endl;
    }

    return 0;
}
