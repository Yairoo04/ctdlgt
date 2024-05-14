#pragma once

#include "structSinhVien.h"
#include "mang.h"
#include "danh_sach_lien_ket_don.h"
#include "danh_sach_lien_ket_vong.h"
#include "danh_sach_lien_ket_kep.h"
#include "nhapXuatSinhVIen.h"

void lietKeSVDiemCaoNhatMang(SV LIST_MANG[], int& soLuongSinhVien)
{
    float maxScore = -1;
    for (int i = 0; i < soLuongSinhVien; i++)
    {
        if (LIST_MANG[i].diem > maxScore)
        {
            maxScore = LIST_MANG[i].diem;
        }
    }

    cout << "\n\t--- Danh sach sinh vien co diem cao nhat\n";
    inTieuDe();

    for (int i = 0; i < soLuongSinhVien; i++)
    {
        if (LIST_MANG[i].diem == maxScore)
        {
            xuat(LIST_MANG[i]);
        }
    }
}

void lietKeSVDiemThapNhatMang(SV LIST_MANG[], int& soLuongSinhVien)
{
    float minScore = 11; // Khởi tạo minScore với giá trị lớn nhất có thể

    for (int i = 0; i < soLuongSinhVien; i++)
    {
        if (LIST_MANG[i].diem < minScore)
        {
            minScore = LIST_MANG[i].diem;
        }
    }

    cout << "\n\t--- Danh sach sinh vien co diem cao nhat\n";
    inTieuDe();

    for (int i = 0; i < soLuongSinhVien; i++)
    {
        if (LIST_MANG[i].diem == minScore)
        {
            xuat(LIST_MANG[i]);
        }
    }
}

void lietKeSVDiemCaoNhatDSLKDon(LIST_DON listDon)
{
    pNODE_DON current = listDon.pHead_Don;
    float maxScore = -1;

    while (current != nullptr)
    {
        if (current->data.diem > maxScore)
        {
            maxScore = current->data.diem;
        }
        current = current->pNext_Don;
    }

    current = listDon.pHead_Don;
    cout << "\n\t--- Danh sach sinh vien co diem cao nhat\n";
    inTieuDe();
    while (current != nullptr)
    {
        if (current->data.diem == maxScore)
        {
            xuat(current->data);
        }
        current = current->pNext_Don;
    }
}

void lietKeSVDiemThapNhatDSLKDon(LIST_DON listDon)
{
    pNODE_DON current = listDon.pHead_Don;
    float minScore = 11; // Khởi tạo minScore với giá trị lớn nhất có thể

    // Tìm điểm thấp nhất
    while (current != nullptr)
    {
        if (current->data.diem < minScore)
        {
            minScore = current->data.diem;
        }
        current = current->pNext_Don;
    }

    current = listDon.pHead_Don;
    cout << "\n\t--- Danh sach sinh vien co diem thap nhat\n";
    inTieuDe();
    // Liệt kê các sinh viên có điểm bằng minScore
    while (current != nullptr)
    {
        if (current->data.diem == minScore)
        {
            xuat(current->data);
        }
        current = current->pNext_Don;
    }
}

void lietKeSVDiemCaoNhatDSLKVong(LIST_VONG listVong)
{
    if (listVong.pTail_Vong == nullptr)
    {
        cout << "Danh sach rong.\n";
        return;
    }

    pNODE_VONG current = listVong.pTail_Vong->pNext_Vong; // Bắt đầu từ đầu danh sách (pTail_Vong->pNext_Vong)
    float maxScore = -1;

    do
    {
        if (current->data.diem > maxScore)
        {
            maxScore = current->data.diem;
        }
        current = current->pNext_Vong;
    } while (current != listVong.pTail_Vong->pNext_Vong); // Duyệt hết vòng

    current = listVong.pTail_Vong->pNext_Vong; // Bắt đầu từ đầu danh sách (pTail_Vong->pNext_Vong)
    cout << "\n\t--- Danh sach sinh vien co diem cao nhat\n";
    inTieuDe();
    do
    {
        if (current->data.diem == maxScore)
        {
            xuat(current->data);
        }
        current = current->pNext_Vong;
    } while (current != listVong.pTail_Vong->pNext_Vong); // Duyệt hết vòng
}

void lietKeSVDiemThapNhatDSLKVong(LIST_VONG listVong)
{
    if (listVong.pTail_Vong == nullptr)
    {
        cout << "Danh sach rong.\n";
        return;
    }

    pNODE_VONG current = listVong.pTail_Vong->pNext_Vong; // Bắt đầu từ đầu danh sách (pTail_Vong->pNext_Vong)
    float minScore = 11;                                  // Khởi tạo minScore với giá trị lớn nhất có thể

    do
    {
        if (current->data.diem < minScore)
        {
            minScore = current->data.diem;
        }
        current = current->pNext_Vong;
    } while (current != listVong.pTail_Vong->pNext_Vong); // Duyệt hết vòng

    current = listVong.pTail_Vong->pNext_Vong; // Bắt đầu từ đầu danh sách (pTail_Vong->pNext_Vong)
    cout << "\n\t--- Danh sach sinh vien co diem cao nhat\n";
    inTieuDe();
    do
    {
        if (current->data.diem == minScore)
        {
            xuat(current->data);
        }
        current = current->pNext_Vong;
    } while (current != listVong.pTail_Vong->pNext_Vong); // Duyệt hết vòng
}

void lietKeSVDiemCaoNhatDSLKKep(LIST_KEP listKep)
{
    if (listKep.pHead_Kep == nullptr)
    {
        cout << "Danh sach rong.\n";
        return;
    }

    pNODE_KEP current = listKep.pHead_Kep;
    float maxScore = -1;

    // Tìm điểm cao nhất trong danh sách
    while (current != nullptr)
    {
        if (current->data.diem > maxScore)
        {
            maxScore = current->data.diem;
        }
        current = current->pNext_Kep;
    }

    // In ra thông tin các sinh viên có điểm bằng với điểm cao nhất
    current = listKep.pHead_Kep;
    cout << "\n\t--- Danh sach sinh vien co diem cao nhat\n";
    inTieuDe();
    while (current != nullptr)
    {
        if (current->data.diem == maxScore)
        {
            xuat(current->data);
        }
        current = current->pNext_Kep;
    }
}

void lietKeSVDiemThapNhatDSLKKep(LIST_KEP listKep)
{
    if (listKep.pHead_Kep == nullptr)
    {
        cout << "Danh sach rong.\n";
        return;
    }

    pNODE_KEP current = listKep.pHead_Kep;
    float minScore = 11; // Khởi tạo minScore với giá trị lớn nhất có thể

    // Tìm điểm cao nhất trong danh sách
    while (current != nullptr)
    {
        if (current->data.diem < minScore)
        {
            minScore = current->data.diem;
        }
        current = current->pNext_Kep;
    }

    // In ra thông tin các sinh viên có điểm bằng với điểm cao nhất
    current = listKep.pHead_Kep;
    cout << "\n\t--- Danh sach sinh vien co diem cao nhat\n";
    inTieuDe();
    while (current != nullptr)
    {
        if (current->data.diem == minScore)
        {
            xuat(current->data);
        }
        current = current->pNext_Kep;
    }
}

float diemTBMang(SV LIST_MANG[], int soLuongSinhVien)
{
    float tongDiem = 0;

    for (int i = 0; i < soLuongSinhVien; i++)
    {
        tongDiem += LIST_MANG[i].diem;
    }

    float diemTB = tongDiem / soLuongSinhVien;
    return diemTB;
}

float diemTBDSLKDon(LIST_DON listDon)
{
    if (listDon.pHead_Don == nullptr)
    {
        cout << "Danh sach rong.\n";
        return 0.0f;
    }

    pNODE_DON current = listDon.pHead_Don;
    float totalScore = 0.0f;
    int numStudents = 0;

    while (current != nullptr)
    {
        totalScore += current->data.diem;
        numStudents++;
        current = current->pNext_Don;
    }

    float classAverage = totalScore / numStudents;
    return classAverage;
}

float diemTBDSLKVong(LIST_VONG listVong)
{
    if (listVong.pTail_Vong == nullptr)
    {
        cout << "Danh sach rong.\n";
        return 0.0f;
    }

    pNODE_VONG current = listVong.pTail_Vong->pNext_Vong; // Bắt đầu từ phần tử đầu tiên sau pTail
    float totalScore = 0.0f;
    int numStudents = 0;

    do
    {
        totalScore += current->data.diem;
        numStudents++;
        current = current->pNext_Vong;
    } while (current != listVong.pTail_Vong->pNext_Vong); // Duyệt đến khi gặp lại phần tử đầu tiên

    float classAverage = totalScore / numStudents;
    return classAverage;
}

float diemTBDSLKKep(LIST_KEP listKep)
{
    if (listKep.pHead_Kep == nullptr || listKep.pTail_Kep == nullptr)
    {
        cout << "Danh sach rong.\n";
        return 0.0f;
    }

    pNODE_KEP current = listKep.pHead_Kep;
    float totalScore = 0.0f;
    int numStudents = 0;

    while (current != nullptr)
    {
        totalScore += current->data.diem;
        numStudents++;
        current = current->pNext_Kep;
    }

    float classAverage = totalScore / numStudents;
    return classAverage;
}
