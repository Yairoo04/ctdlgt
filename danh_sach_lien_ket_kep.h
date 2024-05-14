#pragma once
#include "structSinhVien.h"

typedef struct node_Kep
{
    SV data;
    node_Kep* pNext_Kep;
    node_Kep* pPrev_Kep;
} NODE_KEP;
typedef NODE_KEP* pNODE_KEP;

typedef struct listKep
{
    pNODE_KEP pHead_Kep;
    pNODE_KEP pTail_Kep;
} LIST_KEP;

LIST_KEP listKep;

// ---------------------------------  tu them

pNODE_KEP khoiTaoNodeKep(SV sv) // ------------ kép
{
    pNODE_KEP p = new NODE_KEP;
    if (p == NULL)
    {
        exit(1);
    }
    p->data = sv;
    p->pNext_Kep = NULL;
    p->pPrev_Kep = NULL;

    return p;
}

void khoiTaoDSLKKep(LIST_KEP& listKep)
{
    listKep.pHead_Kep = NULL;
    listKep.pTail_Kep = NULL;
}