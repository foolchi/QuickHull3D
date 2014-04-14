#ifndef VERTEXLIST_H
#define VERTEXLIST_H

#include "vertex.h"

class VertexList{
private:
    Vertex *head, *tail;

public:
    VertexList(){
        head = NULL;
        tail = NULL;
    }

    void clear(){
        head = NULL;
        tail = NULL;
    }

    void add(Vertex *vtx){
        if (head == NULL){
            head = vtx;
        }
        else{
            tail->next = vtx;
        }
        vtx->prev = tail;
        vtx->next = NULL;
        tail = vtx;
    }

    void addAll(Vertex *vtx){
        if (head == NULL){
            head = vtx;
        }
        else{
            tail->next = vtx;
        }
        vtx->prev = tail;

        while (vtx->next != NULL){
            vtx = vtx->next;
        }
        tail = vtx;
    }

    void remove(Vertex *vtx){
        if (vtx == NULL){
            throw("Exception");
        }

        if (vtx->prev == NULL){
            head = vtx->next;
        }
        else{
            vtx->prev->next = vtx->next;
        }
        if (vtx->next == NULL){
            tail = vtx->prev;
        }
        else{
            vtx->next->prev = vtx->prev;
        }
    }

    void removeAll(Vertex *vtx1, Vertex *vtx2){

        if (vtx1->prev == NULL){
            head = vtx2->next;
        }
        else{
            vtx1->prev->next = vtx2->next;
        }
        if (vtx2->next == NULL){
            tail = vtx1->prev;
        }
        else{
            vtx2->next->prev = vtx1->prev;
        }
    }

    void insertBefore(Vertex *vtx, Vertex *vnext){

        if (vtx == NULL || vnext == NULL){
            throw("Exception");
        }

        vtx->prev = vnext->prev;
        if (vnext->prev == NULL){
            head = vtx;
        }
        else{
            vnext->prev->next = vtx;
        }

        vtx->next = vnext;
        vnext->prev = vtx;

    }

    Vertex* first(){
        return head;
    }

    bool isEmpty(){
        return head == NULL;
    }

};

#endif // VERTEXLIST_H
