#include "facet.h"
#include "halfedge.h"
#include "facelist.h"

using namespace std;

Facet::Facet(){
    outside = NULL;
    he0 = NULL;
    next = NULL;
    normal = new Vector3D();
    centroid = new Point3D();
    mark = VISIBLE;
}

void Facet::computeCentroid(Point3D *centroid){
    centroid->setZero();
    HalfEdge *he = he0;
    do{
        centroid->add(*(he->head()->pnt));
        he = he->next;
    }
    while (he != he0);
    centroid->scale(1.0d/numVerts);
}

void Facet::computeNormal(Vector3D *normal, double minArea){
    computeNormal(normal);
    if (area < minArea){
        HalfEdge *hedgeMax = NULL;
        double lenSqrMax = 0;
        HalfEdge *hedge = he0;
        do{
            double lenSqr = hedge->lengthSquared();
            if (lenSqr > lenSqrMax){
                lenSqrMax  = lenSqr;
                hedgeMax = hedge;
            }
            hedge = hedge->next;
        }
        while (hedge != he0);

        Point3D *p2 = hedgeMax->head()->pnt;
        Point3D *p1 = hedgeMax->tail()->pnt;

        double lenMax = sqrt(lenSqrMax);
        double ux = (p2->x - p1->x)/lenMax,
               uy = (p2->y - p1->y)/lenMax,
               uz = (p2->z - p1->z)/lenMax;
        double dot = normal->x * ux + normal->y * uy + normal->z * uz;
        normal->x -= dot * ux;
        normal->y -= dot * uy;
        normal->z -= dot * uz;

        normal->normalize();
    }
}

void Facet::computeNormal(Vector3D *normal){
    HalfEdge *he1 = he0->next;
    HalfEdge *he2 = he1->next;

    Point3D *p0 = he0->head()->pnt;
    Point3D *p2 = he1->head()->pnt;

    double d2x = p2->x - p0->x,
           d2y = p2->y - p0->y,
           d2z = p2->z - p0->z;

    normal->setZero();
    numVerts = 2;

    while (he2 != he0){
        double d1x = d2x,
               d1y = d2y,
               d1z = d2z;
        p2 = he2->head()->pnt;
        d2x = p2->x - p0->x;
        d2y = p2->y - p0->y;
        d2z = p2->z - p0->z;

        normal->x += d1y * d2z - d1z * d2y;
        normal->y += d1z * d2x - d1x * d2z;
        normal->z += d1x * d2y - d1y * d2x;

        he1 = he2;
        he2 = he2->next;
        numVerts++;
    }
    area = normal->norm();
    normal->scale(1.0d/area);
}

void Facet::computeNormalAndCentroid(){
    computeNormal(normal);
    computeCentroid(centroid);
    planeOffset = normal->dot(*centroid);
    int numV = 0;
    HalfEdge *he = he0;
    do{
        numV++;
        he = he->next;
    }
    while (he != he0);

    if (numV != numVerts){
        qDebug("Error in computeNormalAndCentroid");
    }
}

void Facet::computeNormalAndCentroid(double minArea){
    computeNormal(normal, minArea);
    computeCentroid(centroid);
    planeOffset = normal->dot(*centroid);
}

Facet* Facet::createTriangle(Vertex *v0, Vertex *v1, Vertex *v2,
                                     double minArea){
    Facet *facet = new Facet();
    HalfEdge *he0 = new HalfEdge(v0, facet);
    HalfEdge *he1 = new HalfEdge(v1, facet);
    HalfEdge *he2 = new HalfEdge(v2, facet);

    he0->prev = he2;
    he0->next = he1;
    he1->prev = he0;
    he1->next = he2;
    he2->prev = he1;
    he2->next = he0;

    facet->he0 = he0;
    facet->computeNormalAndCentroid(minArea);
    return facet;
}

Facet* Facet::create(vector<Vertex*> vtxarray, vector<int> indices){
    Facet *facet = new Facet();
    HalfEdge *hePrev = NULL;
    for (int i = 0; i < indices.size(); i++){
        HalfEdge *he = new HalfEdge(vtxarray[indices[i]], facet);
        if (hePrev != NULL){
            he->setPrev(hePrev);
            hePrev->setNext(he);
        }
        else{
            facet->he0 = he;
        }
        hePrev = he;
    }
    facet->he0->setPrev(hePrev);
    hePrev->setNext(facet->he0);

    facet->computeNormalAndCentroid();
    return facet;
}

HalfEdge* Facet::getEdge(int i){
    HalfEdge *he = he0;
    while (i > 0){
        he = he->next;
        i--;
    }
    while (i < 0){
        he = he->prev;
        i++;
    }
    return he;
}

HalfEdge* Facet::getFirstEdge(){
    return he0;
}

HalfEdge* Facet::findEdge(Vertex *vt, Vertex *vh){
    HalfEdge *he = he0;
    do{
        if (he->head() == vh && he->tail() == vt)
            return he;
        he = he->next;
    }while (he != he0);

    return NULL;
}

double Facet::distanceToPlane(Point3D *p){
    return normal->x * p->x +
           normal->y * p->y +
           normal->z * p->z - planeOffset;
}

Vector3D* Facet::getNormal(){
    return normal;
}

Point3D* Facet::getCentroid(){
    return centroid;
}

int Facet::numVertices(){
    return numVerts;
}

vector<int> Facet::getVertexIndices(){
    HalfEdge *he = he0;
    vector<int> idxs;
    do{
        idxs.push_back(he->head()->index);
        he = he->next;
    }
    while (he != he0);
    return idxs;
}

Facet* Facet::connectHalfEdges(HalfEdge *hedgePrev, HalfEdge *hedge){
    Facet *discardedFacet = NULL;
    if (hedgePrev->oppositeFace() == hedge->oppositeFace()){
        Facet *oppFacet = hedge->oppositeFace();
        HalfEdge *hedgeOpp;

        if (hedgePrev == he0){
            he0 = hedge;
        }
        if (oppFacet->numVertices() == 3){
            hedgeOpp = hedge->getOpposite()->prev->getOpposite();
            oppFacet->mark = DELETED;
            discardedFacet = oppFacet;
        }
        else{
            hedgeOpp = hedge->getOpposite()->next;
            if (oppFacet->he0 == hedgeOpp->prev)
                oppFacet->he0 = hedgeOpp;
            hedgeOpp->prev = hedgeOpp->prev->prev;
            hedgeOpp->prev->next = hedgeOpp;
        }

        hedge->prev = hedgePrev->prev;
        hedge->prev->next = hedge;
        hedge->opposite = hedgeOpp;
        hedgeOpp->opposite = hedge;
        oppFacet->computeNormalAndCentroid();
    }
    else{
        hedgePrev->next = hedge;
        hedge->prev = hedgePrev;
    }
    return discardedFacet;
}

void Facet::checkConsistency(){
    //return;

    HalfEdge *hedge = he0;
    //double maxd = 0;
    int numV = 0;

    if (numVerts < 3){
        qDebug("numVerts < 3, Error in checkConsistency, Stop");
    }
    do{
        HalfEdge *hedgeOpp = hedge->getOpposite();
        if (hedgeOpp == NULL){
            qDebug("hedgeOpp is NULL,Error in checkConsistency, Stop");
        }
        else if (hedgeOpp->getOpposite() != hedge){
            qDebug("hedge's Opp's opp != hedge, Error in checkConsistency, Stop");

        }
        if (hedgeOpp->head() != hedge->tail() ||
                hedge->head() != hedgeOpp->tail()){
            qDebug("hedgeOpp.head != hedge.tail, Error in checkConsistency, Stop");
            HalfEdge *he = he0;
            do {
                qDebug("Index: %d", he->head()->index);
                he = he->next;
            }while (he != he0);
        }
        Facet *oppFace = hedgeOpp->facet;
        if (oppFace == NULL){
            qDebug("oppFace is NULL, Error in checkConsistency, Stop");

        }
        else if (oppFace->mark == DELETED){
            qDebug("oppFace is deleted, Error in checkConsistency, Stop");
            HalfEdge *he = he0;
            do {
                qDebug("Index: %d", he->head()->index);
                he = he->next;
            }while (he != he0);
        }
        numV++;

        hedge = hedge->next;
    }while (hedge != he0);

    if (numV != numVerts){
        qDebug("numV(%d) != numVerts(%d), Error in checkConsistency, Stop", numV, numVerts);
    }
}

int Facet::mergeAdjacentFace(HalfEdge *hedgeAdj,
                             vector<Facet*> *discarded){

    Facet *oppFace = hedgeAdj->oppositeFace();

    discarded->clear();
    oppFace->mark = DELETED;
    discarded->push_back(oppFace);

    HalfEdge *hedgeOpp = hedgeAdj->getOpposite();
    HalfEdge *hedgeAdjPrev = hedgeAdj->prev;
    HalfEdge *hedgeAdjNext = hedgeAdj->next;
    HalfEdge *hedgeOppPrev = hedgeOpp->prev;
    HalfEdge *hedgeOppNext = hedgeOpp->next;

    while (hedgeAdjPrev->oppositeFace() == oppFace){
        hedgeAdjPrev = hedgeAdjPrev->prev;
        hedgeOppNext = hedgeOppNext->next;
    }

    while (hedgeAdjNext->oppositeFace() == oppFace){
        hedgeOppPrev = hedgeOppPrev->prev;
        hedgeAdjNext = hedgeAdjNext->next;
    }

    HalfEdge *hedge;

    for (hedge = hedgeOppNext;
         hedge != hedgeOppPrev->next; hedge = hedge->next){
        hedge->facet = this;
    }

    if (hedgeAdj == he0)
        he0 = hedgeAdjNext;

    Facet *discardedFace;

    discardedFace = connectHalfEdges(hedgeOppPrev, hedgeAdjNext);
    if (discardedFace != NULL){
        discarded->push_back(discardedFace);
    }

    discardedFace = connectHalfEdges(hedgeAdjPrev, hedgeOppNext);
    if (discardedFace != NULL){
        discarded->push_back(discardedFace);
    }

    computeNormalAndCentroid();
    checkConsistency();


    return discarded->size();
}


void Facet::triangulate(FaceList *newFaces, double minArea){
    HalfEdge *hedge;
    if (numVertices() < 4){
        return;
    }

    Vertex *v0 = he0->head();

    hedge = he0->next;
    HalfEdge *oppPrev = hedge->opposite;
    Facet *facet0 = NULL;

    for (hedge = hedge->next;
         hedge != he0->prev; hedge = hedge->next){
        Facet *facet = createTriangle(v0, hedge->prev->head(),
                                      hedge->head(), minArea);
        facet->he0->next->setOpposite(oppPrev);
        facet->he0->prev->setOpposite(hedge->opposite);
        oppPrev = facet->he0;
        newFaces->add(facet);
        if (facet0 == NULL)
            facet0 = facet;
        hedge = new HalfEdge(he0->prev->prev->head(), this);
        hedge->setOpposite(oppPrev);

        hedge->prev = he0;
        hedge->prev->next = hedge;
        hedge->next = he0->prev;
        hedge->next->prev = hedge;

        computeNormalAndCentroid(minArea);
        checkConsistency();

        for (Facet *facet = facet0;
             facet != NULL; facet = facet->next){
            facet->checkConsistency();
        }
    }
}
