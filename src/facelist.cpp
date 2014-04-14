#include "facelist.h"

FaceList::FaceList(){
    head = tail = NULL;
}

void FaceList::clear(){
    head = tail = NULL;
}

void FaceList::add(Facet *vtx){
    if (head == NULL)
        head = vtx;
    else
        tail->next = vtx;
    vtx->next = NULL;
    tail = vtx;
}

Facet* FaceList::first(){
    return head;
}

bool FaceList::isEmpty(){
    return head == NULL;
}
