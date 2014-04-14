#include "halfedge.h"


HalfEdge::HalfEdge(){
    prev = next = opposite = NULL;
    facet = NULL;
}

HalfEdge::HalfEdge(Vertex *v, Facet *f){
    prev = next = opposite = NULL;
    vertex = v;
    facet = f;
}

void HalfEdge::setVertexAndFacet(Vertex *v, Facet *f){
    vertex = v;
    facet = f;
}

void HalfEdge::setNext(HalfEdge *edge){
    next = edge;
}

HalfEdge* HalfEdge::getNext(){
    return next;
}

void HalfEdge::setPrev(HalfEdge *edge){
    prev = edge;
}

HalfEdge* HalfEdge::getPrev(){
    return prev;
}

Facet* HalfEdge::getFace(){
    return facet;
}

HalfEdge* HalfEdge::getOpposite(){
    return opposite;
}

void HalfEdge::setOpposite(HalfEdge* edge){

    opposite = edge;
    edge->opposite = this;

}

Vertex* HalfEdge::head(){
    return vertex;
}

Vertex* HalfEdge::tail(){
    return prev != NULL ? prev->vertex : NULL;
}

Facet* HalfEdge::oppositeFace(){

    if (opposite != NULL){

        return opposite->facet;
    }
    else{

        return NULL;
    }
}

double HalfEdge::length(){
    if (tail() != NULL){
        return head()->pnt->distance(*(tail()->pnt));
    }
    else{
        return -1;
    }
}

double HalfEdge::lengthSquared(){
    if (tail() != NULL){
        return head()->pnt->distanceSquared(*(tail()->pnt));
    }
    else{
        return -1;
    }
}

