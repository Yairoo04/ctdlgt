#pragma once
#include "structSinhVien.h"
typedef struct node_Don
{
    SV data;
    node_Don* pNext_Don;
} NODE_DON;

typedef NODE_DON* pNODE_DON;

typedef struct listDon
{
    pNODE_DON pHead_Don;
} LIST_DON;

LIST_DON listDon;

// ---------------------------------  tu them
pNODE_DON khoiTaoNodeDon(SV sv) // ----------- ĐƠN
{
    pNODE_DON p = new NODE_DON;
    if (p == NULL)
    {
        exit(1);
    }
    p->data = sv;
    p->pNext_Don = NULL;

    return p;
}

void khoitaoDSLKDon(LIST_DON& listDon)
{
    listDon.pHead_Don = NULL;
}