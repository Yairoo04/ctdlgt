#pragma once
#include "structSinhVien.h"

typedef struct node_Vong
{
    SV data;
    node_Vong* pNext_Vong;
} NODE_VONG;
typedef NODE_VONG* pNODE_VONG;

typedef struct listVong
{
    pNODE_VONG pTail_Vong; // Con tro pTail tham chiếu đến phần tử cuối cùng của danh sách
} LIST_VONG;

LIST_VONG listVong;

// ----------------- tu them

pNODE_VONG khoiTaoNodeVong(SV sv) // ----------- vòng
{
    pNODE_VONG p = new NODE_VONG;
    if (p == NULL)
    {
        exit(1);
    }
    p->data = sv;
    p->pNext_Vong = NULL;

    return p;
}

void khoiTaoDSLKVong(LIST_VONG& listVong)
{
    listVong.pTail_Vong = NULL; // Khởi tạo pTail là NULL khi danh sách rỗng
}