#pragma once

#include "structSinhVien.h"
#include "mang.h"
#include "danh_sach_lien_ket_don.h"
#include "danh_sach_lien_ket_vong.h"
#include "danh_sach_lien_ket_kep.h"

void xoaSVMang(SV LIST_MANG[], int& soLuongSinhVien)
{
    string mssvCanXoa = "";
    cout << "\n\t- Nhap ma sinh vien can xoa : ";
    getline(cin, mssvCanXoa);
    formMssv(mssvCanXoa);

    int index = -1;
    for (int i = 0; i < soLuongSinhVien; i++)
    {
        if (LIST_MANG[i].maSV == mssvCanXoa)
        {
            index = i;
            break;
        }
    }

    if (index == -1)
    {
        cout << "\n\t-- Khong tim thay sinh vien can xoa!!.\n";
        return;
    }

    for (int i = index; i < soLuongSinhVien - 1; i++)
    {
        LIST_MANG[i] = LIST_MANG[i + 1];
    }

    soLuongSinhVien--;
}

void xoaSVDSLKDon(LIST_DON& listDon)
{
    string mssvCanXoa = "";
    cout << "\n\t- Nhap ma sinh vien can xoa : ";
    getline(cin, mssvCanXoa);
    formMssv(mssvCanXoa);

    pNODE_DON current = listDon.pHead_Don;
    pNODE_DON previous = nullptr;

    while (current != nullptr)
    {
        if (current->data.maSV == mssvCanXoa)
        { // Giả sử `data` là nơi lưu trữ thông tin sinh viên, trong đó có `mssv`
            if (previous == nullptr)
            {
                listDon.pHead_Don = current->pNext_Don;
            }
            else
            {
                previous->pNext_Don = current->pNext_Don;
            }
            delete current;
            cout << "\n\tDa xoa sinh vien co ma SV " << mssvCanXoa << " khoi danh sach.\n";
            return;
        }
        previous = current;
        current = current->pNext_Don;
    }

    cout << "\n\tKhong tim thay sinh vien co ma SV " << mssvCanXoa << " trong danh sach.\n";
}

void xoaSVDSLKVong(LIST_VONG& listVong)
{
    string mssvCanXoa = "";
    cout << "\n\t- Nhap ma sinh vien can xoa : ";
    getline(cin, mssvCanXoa);
    formMssv(mssvCanXoa);

    if (listVong.pTail_Vong == nullptr)
    {
        cout << "\n\tDanh sach rong.\n";
        return;
    }

    pNODE_VONG current = listVong.pTail_Vong->pNext_Vong; // Bắt đầu từ đầu danh sách (pTail->pNext_Vong)
    pNODE_VONG previous = listVong.pTail_Vong;

    do
    {
        if (current->data.maSV == mssvCanXoa)
        {
            if (current == previous)
            { // Nếu danh sách chỉ có một node
                listVong.pTail_Vong = nullptr;
            }
            else
            {
                previous->pNext_Vong = current->pNext_Vong;
                if (current == listVong.pTail_Vong)
                { // Nếu node cần xóa là node cuối
                    listVong.pTail_Vong = previous;
                }
            }
            delete current;
            cout << "\n\tDa xoa sinh vien co ma SV " << mssvCanXoa << " khoi danh sach.\n";
            return;
        }
        previous = current;
        current = current->pNext_Vong;
    } while (current != listVong.pTail_Vong->pNext_Vong); // Duyệt hết vòng

    cout << "\n\tKhong tim thay sinh vien co ma SV " << mssvCanXoa << " trong danh sach.\n";
}

void xoaSVDSLKKep(LIST_KEP& listKep)
{
    string mssvCanXoa = "";
    cout << "\n\t- Nhap ma sinh vien can xoa : ";
    getline(cin, mssvCanXoa);
    formMssv(mssvCanXoa);

    if (listKep.pHead_Kep == nullptr)
    {
        cout << "Danh sach rong.\n";
        return;
    }

    NODE_KEP* current = listKep.pHead_Kep;

    while (current != nullptr)
    {
        if (current->data.maSV == mssvCanXoa)
        {
            // Nếu node cần xóa là node đầu tiên
            if (current == listKep.pHead_Kep)
            {
                listKep.pHead_Kep = current->pNext_Kep;
                if (listKep.pHead_Kep != nullptr)
                {
                    listKep.pHead_Kep->pPrev_Kep = nullptr;
                }
                else
                {
                    listKep.pTail_Kep = nullptr; // Danh sách trở nên rỗng
                }
            }
            // Nếu node cần xóa là node cuối cùng
            else if (current == listKep.pTail_Kep)
            {
                listKep.pTail_Kep = current->pPrev_Kep;
                listKep.pTail_Kep->pNext_Kep = nullptr;
            }
            // Nếu node cần xóa là node ở giữa
            else
            {
                current->pPrev_Kep->pNext_Kep = current->pNext_Kep;
                current->pNext_Kep->pPrev_Kep = current->pPrev_Kep;
            }

            delete current;
            cout << "\n\tDa xoa sinh vien co ma SV " << mssvCanXoa << " khoi danh sach.\n";
            return;
        }
        current = current->pNext_Kep;
    }

    cout << "\n\tKhong tim thay sinh vien co ma SV " << mssvCanXoa << " trong danh sach.\n";
}
