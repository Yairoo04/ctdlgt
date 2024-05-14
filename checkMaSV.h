#pragma once

#include "structSinhVien.h"
#include "mang.h"
#include "danh_sach_lien_ket_don.h"
#include "danh_sach_lien_ket_vong.h"

SV LIST_MANG[200];

bool checkMaSv(string maSv, int luaChon) {
	// Neu luu tru kieu mang thi kiem tra
	if (luaChon == 1) {
		for (int i = 0; i < soLuong; i++) {
			if (maSv == LIST_MANG[i].maSV) // So sánh chuỗi sử dụng toán tử ==
				return false;
		}
	}

	// Neu luu tru kieu lien ket don hoac kep thi kiem tra
	if (luaChon == 2) {
		pNODE_DON p = listDon.pHead_Don; // Ds da duoc khoi tao truoc do
		while (p != NULL) {
			if (maSv == p->data.maSV) // So sánh chuỗi sử dụng toán tử ==
				return false;
			p = p->pNext_Don;
		}
	}

	// Neu luu tru kieu lien ket vong thi kiem tra
	if (luaChon == 3) {
		pNODE_VONG p = listVong.pTail_Vong;
		while (p != NULL) { // Thay doi dieu kien dung cua vong lap
			if (maSv == p->data.maSV) // So sánh chuỗi sử dụng toán tử ==
				return false;
			p = p->pNext_Vong;
		}
	}

	// Neu luu tru kieu lien ket kep thi kiem tra
	if (luaChon == 4) {
		pNODE_KEP p = listKep.pTail_Kep;
		while (p != NULL) { // Thay doi dieu kien dung cua vong lap
			if (maSv == p->data.maSV) // So sánh chuỗi sử dụng toán tử ==
				return false;
			p = p->pNext_Kep;
		}
	}
	// kep
	return true;
}
