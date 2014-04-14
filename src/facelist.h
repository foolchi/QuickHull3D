#ifndef FACELIST_H
#define FACELIST_H
/**
 * @file facelist.h
 * @brief Single-list of face
 */

#include "facet.h"

/**
 * @brief Single-list of face
 */
class FaceList{

private:
    Facet *head;
    Facet *tail;

public:
    FaceList();
    void clear();
    void add(Facet *vtx);
    Facet *first();
    bool isEmpty();

};

#endif // FACELIST_H
