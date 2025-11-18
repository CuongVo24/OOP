#include <iostream>
#include <cstring>
#include <algorithm>
#include "SLList.h"

using namespace std;

class SoNguyenLon {
private:
    SLList<int> chuSo;
    int dau;

    void trim() {
        while (chuSo.size() > 1 && chuSo.back() == 0) {
            chuSo.RemoveTail();
        }
    }

    int compareAbs(const SoNguyenLon& other) const {
        int n1 = this->chuSo.size();
        int n2 = other.chuSo.size();
        if (n1 != n2) return (n1 > n2) ? 1 : -1;

        int* a1 = new int[n1];
        int* a2 = new int[n2];
        int i = 0;

        auto it1 = const_cast<SLList<int>&>(this->chuSo).begin();
        for (int k = 0; k < n1; ++k, ++it1) a1[k] = *it1;

        auto it2 = const_cast<SLList<int>&>(other.chuSo).begin();
        for (int k = 0; k < n2; ++k, ++it2) a2[k] = *it2;

        int result = 0;
        for (int k = n1 - 1; k >= 0; k--) {
            if (a1[k] > a2[k]) { result = 1; break; }
            if (a1[k] < a2[k]) { result = -1; break; }
        }

        delete[] a1;
        delete[] a2;
        return result;
    }

    SLList<int> subtractAbs(const SLList<int>& lon, const SLList<int>& be) const {
        SLList<int> ketQua;
        auto itL = const_cast<SLList<int>&>(lon).begin();
        auto itB = const_cast<SLList<int>&>(be).begin();
        auto endL = const_cast<SLList<int>&>(lon).end();
        auto endB = const_cast<SLList<int>&>(be).end();

        int muon = 0;
        while (itL != endL) {
            int valL = *itL;
            int valB = (itB != endB) ? *itB : 0;

            int hieu = valL - valB - muon;
            if (hieu < 0) {
                hieu += 10;
                muon = 1;
            }
            else {
                muon = 0;
            }

            ketQua.push_back(hieu);

            ++itL;
            if (itB != endB) ++itB;
        }
        return ketQua;
    }

    SLList<int> addAbs(const SLList<int>& a, const SLList<int>& b) const {
        SLList<int> ketQua;
        auto it1 = const_cast<SLList<int>&>(a).begin();
        auto it2 = const_cast<SLList<int>&>(b).begin();
        auto end1 = const_cast<SLList<int>&>(a).end();
        auto end2 = const_cast<SLList<int>&>(b).end();

        int nho = 0;
        while (it1 != end1 || it2 != end2) {
            int val1 = (it1 != end1) ? *it1 : 0;
            int val2 = (it2 != end2) ? *it2 : 0;
            int tong = val1 + val2 + nho;
            ketQua.push_back(tong % 10);
            nho = tong / 10;

            if (it1 != end1) ++it1;
            if (it2 != end2) ++it2;
        }
        if (nho > 0) ketQua.push_back(nho);
        return ketQua;
    }

public:
    SoNguyenLon() : dau(1) { chuSo.push_back(0); }

    SoNguyenLon(const char* s) {
        int i = 0;
        dau = 1;
        if (s[0] == '-') {
            dau = -1;
            i = 1;
        }
        else if (s[0] == '+') {
            i = 1;
        }

        int len = strlen(s);
        for (int j = len - 1; j >= i; j--) {
            if (s[j] >= '0' && s[j] <= '9') {
                chuSo.push_back(s[j] - '0');
            }
        }
        if (chuSo.empty()) chuSo.push_back(0);
        trim();
        if (chuSo.size() == 1 && chuSo.back() == 0) dau = 1;
    }

    friend ostream& operator<<(ostream& os, SoNguyenLon& snl) {
        if (snl.dau == -1) os << "-";

        int n = snl.chuSo.size();
        int* temp = new int[n];
        int i = 0;
        for (auto it = snl.chuSo.begin(); it != snl.chuSo.end(); ++it) temp[i++] = *it;

        for (int j = n - 1; j >= 0; j--) os << temp[j];

        delete[] temp;
        return os;
    }

    friend istream& operator>>(istream& is, SoNguyenLon& snl) {
        char buffer[10000];
        is >> buffer;
        snl = SoNguyenLon(buffer);
        return is;
    }

    SoNguyenLon operator+(const SoNguyenLon& other) {
        SoNguyenLon kq;

        if (this->dau == other.dau) {
            kq.dau = this->dau;
            kq.chuSo = addAbs(this->chuSo, other.chuSo);
        }
        else {
            int cmp = compareAbs(other);
            if (cmp >= 0) {
                kq.dau = this->dau;
                kq.chuSo = subtractAbs(this->chuSo, other.chuSo);
            }
            else {
                kq.dau = other.dau;
                kq.chuSo = subtractAbs(other.chuSo, this->chuSo);
            }
        }
        kq.trim();

        if (kq.chuSo.size() == 1 && kq.chuSo.back() == 0) kq.dau = 1;

        return kq;
    }

    SoNguyenLon operator-(const SoNguyenLon& other) {
        SoNguyenLon soDoi = other;
        soDoi.dau = -soDoi.dau;
        return *this + soDoi;
    }
};
