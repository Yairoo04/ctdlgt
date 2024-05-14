#pragma once

#include "structSinhVien.h"
#include <string.h>
#include <algorithm>
#include "danh_sach_lien_ket_don.h"
#include "checkMaSV.h"
#include "mang.h"
#include "danh_sach_lien_ket_kep.h"
#include "danh_sach_lien_ket_vong.h"

// định dạng form chữ
string formChu(string& str) // chỉ dùng cho chữ , ko được dùng số
{
    while (str[0] == ' ')
    {
        str.erase(str.begin() + 0);
    }

    while (str[str.length() - 1] == ' ')
    {
        // xóa kí tự tại vị trí 0
        str.erase(str.begin() + str.length() - 1);
    }

    for (int i = 0; i < str.length() - 1; i++)
    {
        if (str[i] == ' ' && str[i + 1] == ' ')
        {
            str.erase(str.begin() + i);
            i--;
        }
    }

    transform(str.begin(), str.end(), str.begin(), ::tolower); // CHUYEN SANG CHU THUONG
    int i = 1;
    for (int i = 0; i < str.length(); i++)
    {
        if (str[i] == ' ')
        {
            str[i + 1] = str[i + 1] - 32;
        }
    }
    str[0] = str[0] - 32;
    return str;
}

string formMssv(string& str)
{
    for (int i = 0; i < str.length(); i++)
    {
        if (str[i] >= 97 && str[i] <= 122)
        {
            str[i] = str[i] - 32;
        }
    }
    return str;
}

SV nhapThongTinSinhVien()
{
    SV sv;
    cin.ignore(); // CHỖ NÀY IGNORE Ở SAU KHI NHẬP SỐ

    cout << "\n\t- Nhap ma SV: ";
    getline(cin, sv.maSV);
    formMssv(sv.maSV);
    cout << "\n\t- Nhap ho SV: ";
    getline(cin, sv.ho);
    formChu(sv.ho);
    cout << "\n\t- Nhap ten SV: ";
    getline(cin, sv.ten);
    formChu(sv.ten);
    cout << "\n\t- Nhap lop cua SV: ";
    getline(cin, sv.lop);
    formMssv(sv.lop);
    cout << "\n\t- Nhap diem cua SV: ";
    cin >> sv.diem;

    return sv;
}

SV nhapTTSinhVien(fstream& input, int luaChon)
{
    soLuong++; // tang so luong sinh vien
    SV sv;
    string temp;

    input >> sv.stt;
    input.ignore();
    getline(input, temp, ',');
    while (!checkMaSv(temp, luaChon))
    {
        cout << "\nMa sinh vien " << temp << " da ton tai\nVui long nhap lai ma sinh vien: ";
        getline(cin, temp);
    }
    sv.maSV = temp;
    getline(input, sv.ho, ',');
    getline(input, sv.ten, ',');
    getline(input, sv.lop, ',');
    input >> sv.diem;

    return sv;
}

// Hàm in tiêu đề
void inTieuDe()
{
    cout << left << setw(30) << "| MSSV" << setw(30) << "| Ho va Ten" << setw(20) << "| Lop" << setw(10) << "| Diem" << endl;
    cout << setfill('-') << setw(90) << "" << setfill(' ') << endl;
}

void xuat(SV sv)
{
    cout << left << setw(32) << sv.maSV << setw(30) << sv.ho + " " + sv.ten << setw(20) << sv.lop << setw(10) << sv.diem << endl;
}

// --------------------------------------- MẢNG

void themSinhVienMang(SV sv[], int& soLuongSinhVienMang, int index)
{

    SV x = nhapThongTinSinhVien();
    if (index >= 0 && index <= soLuongSinhVienMang)
    {
        for (int i = soLuongSinhVienMang; i > index; i--)
            sv[i] = sv[i - 1];
        sv[index] = x;
        soLuongSinhVienMang++;
    }
}

void xuatSinhVienMang(SV LIST_MANG[], int& soLuongSinhVienMang)
{
    for (int i = 0; i < soLuongSinhVienMang; i++)
    {
        xuat(LIST_MANG[i]);
        cout << endl;
    }
}

// --------------------------- DSLK ĐƠN
// Hàm thêm vào cuối DSLK Dơn
void themVaoCuoiDSLKDon(LIST_DON& listDon, pNODE_DON p)
{
    if (listDon.pHead_Don == NULL)
    {
        listDon.pHead_Don = p;
    }
    else
    {
        pNODE_DON temp = listDon.pHead_Don;
        while (temp->pNext_Don != NULL)
        {
            temp = temp->pNext_Don;
        }
        temp->pNext_Don = p;
    }
}

void themSVVaoCuoiDSLKDon(LIST_DON& listDon, const SV& sv)
{
    pNODE_DON p = khoiTaoNodeDon(sv);
    themVaoCuoiDSLKDon(listDon, p);
}

void xuatDSLKDon(LIST_DON listDon)
{
    for (pNODE_DON p = listDon.pHead_Don; p != NULL; p = p->pNext_Don)
    {
        xuat(p->data);
    }
}

// Ham them vao cuoi
void themVaoCuoiDSLKVong(LIST_VONG& listVong, pNODE_VONG p)
{
    // Danh sach rong
    if (listVong.pTail_Vong == NULL)
    {
        listVong.pTail_Vong = p;
        p->pNext_Vong = p; // Khi danh sách rỗng, pNext của phần tử đầu tiên trỏ lại chính nó
    }
    else
    {
        p->pNext_Vong = listVong.pTail_Vong->pNext_Vong; // Liên kết phần tử mới với phần tử đầu tiên
        listVong.pTail_Vong->pNext_Vong = p;             // Liên kết phần tử cuối cùng với phần tử mới
        listVong.pTail_Vong = p;                         // Cập nhật pTail để trỏ đến phần tử mới
    }
}

// void themSVVaoCuoiDSLKVong(LIST_VONG& listVong, const SV& sv)
//{
//     cout << "--t";
//     NODE_VONG* p = new NODE_VONG;
//     if (p == nullptr)
//     {
//         throw std::bad_alloc();
//     }
//     p->data = sv;
//     p->pNext_Vong = nullptr;
//     themVaoCuoiDSLKVong(listVong, p);
// }

void themSVVaoCuoiDSLKVong(LIST_VONG& listVong, const SV& sv)
{
    pNODE_VONG p = khoiTaoNodeVong(sv);
    themVaoCuoiDSLKVong(listVong, p);
}

void xuatDSLKVong(LIST_VONG& listVong)
{
    if (listVong.pTail_Vong != nullptr)
    {
        pNODE_VONG p = listVong.pTail_Vong->pNext_Vong;
        do
        {
            xuat(p->data);
            p = p->pNext_Vong;
        } while (p != listVong.pTail_Vong->pNext_Vong);
    }
}

// Ham them vao cuoi
void themVaoCuoiDSLKKep(LIST_KEP& listKep, pNODE_KEP p)
{
    // Danh sach rong
    if (listKep.pHead_Kep == NULL)
    {
        listKep.pHead_Kep = listKep.pTail_Kep = p;
    }
    else
    {
        listKep.pTail_Kep->pNext_Kep = p;
        p->pPrev_Kep = listKep.pTail_Kep;
        listKep.pTail_Kep = p;
    }
}

void themSVVaoCuoiDSLKKep(LIST_KEP& listKep, const SV& sv)
{
    pNODE_KEP p = khoiTaoNodeKep(sv);
    themVaoCuoiDSLKKep(listKep, p);
}

void xuatDSLKKep(LIST_KEP listKep)
{
    for (pNODE_KEP p = listKep.pHead_Kep; p != NULL; p = p->pNext_Kep)
    {
        xuat(p->data);
    }
}

void nhapMang()
{
    fstream input("DSSV.csv", ios::in | ios::out);

    if (!input.is_open())
    {
        cout << "\nMo file khong thanh cong\n";
        return;
    }

    // Đọc dòng tiêu đề
    string temp;
    getline(input, temp);

    int i = 0;
    while (!input.eof())
    {
        LIST_MANG[i] = nhapTTSinhVien(input, 1);
        i++;
    }

    input.close();
}

// doc vao danh sach lien ket don
void nhapDon(LIST_DON& listDon)
{
    fstream input("DSSV.csv", ios::in | ios::out);

    if (!input.is_open())
    {
        cout << "\nMo file khong thanh cong\n";
        return;
    }

    // Đọc dòng tiêu đề
    string temp;
    getline(input, temp);

    while (!input.eof())
    {
        themSVVaoCuoiDSLKDon(listDon, nhapTTSinhVien(input, 2));
    }
}

// doc vao danh sach lien ket vong
void nhapVong(LIST_VONG& listVong)
{
    fstream input("DSSV.csv", ios::in | ios::out);

    if (!input.is_open())
    {
        cout << "\nMo file khong thanh cong\n";
        return;
    }

    // Đọc dòng tiêu đề
    string temp;
    getline(input, temp);

    while (!input.eof())
    {
        SV sv = nhapTTSinhVien(input, 3);
        pNODE_VONG p = new NODE_VONG;
        p->data = sv;
        p->pNext_Vong = NULL;
        themVaoCuoiDSLKVong(listVong, p);
    }
}
void nhapKep(LIST_KEP& listKep)
{
    fstream input("DSSV.csv", ios::in | ios::out);

    if (!input.is_open())
    {
        cout << "\nMo file khong thanh cong\n";
        return;
    }

    // Đọc dòng tiêu đề
    string temp;
    getline(input, temp);

    while (!input.eof())
    {
        SV sv = nhapTTSinhVien(input, 4);
        themSVVaoCuoiDSLKKep(listKep, sv);
    }
}
