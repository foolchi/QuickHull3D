#include "quickhull3d.h"

using namespace std;

void QuickHull3D::build(double *coords, int nump){

    if (nump < 4){
        qDebug("Less than 4 points, break");
        return;
    }

    for (int i = 0; i < nump; i++){
        pointBuffer.push_back(new Vertex());
    }
    for (int i = 0; i < 3; i++){
        maxVtxs.push_back(new Vertex());
        minVtxs.push_back(new Vertex());
    }
    initBuffers(nump);
    setPoints(coords, nump);
    buildHull();

}

void QuickHull3D::build(double **coords, int nump){
    if (nump < 4){
        qDebug("Less than 4 points, break");
        return;
    }

    for (int i = 0; i < nump; i++){
        pointBuffer.push_back(new Vertex());
    }
    for (int i = 0; i < 3; i++){
        maxVtxs.push_back(new Vertex());
        minVtxs.push_back(new Vertex());
    }
    initBuffers(nump);
    setPoints(coords, nump);
    buildHull();
}

QuickHull3D::QuickHull3D(){
    newFaces = new FaceList();
    unclaimed = new VertexList();
    claimed = new VertexList();
    explicitTolerance = AUTOMATIC_TOLERANCE;
}

QuickHull3D::QuickHull3D(double* coords, int len){
    newFaces = new FaceList();
    unclaimed = new VertexList();
    claimed = new VertexList();
    explicitTolerance = AUTOMATIC_TOLERANCE;
    if (len % 3 == 0)
        build(coords, len/3);
    else
        qDebug("Wrong length");
}

QuickHull3D::QuickHull3D(double** coords, int len){
    newFaces = new FaceList();
    unclaimed = new VertexList();
    claimed = new VertexList();
    explicitTolerance = AUTOMATIC_TOLERANCE;
    build(coords, len);

}


double QuickHull3D::getDistanceTolerance(){
    return tolerance;
}

void QuickHull3D::initBuffers(int nump){
    faces.clear();
    claimed->clear();
    numFaces = 0;
    numPoints = nump;
}

void QuickHull3D::setExplicitDistanceTolerance(double tol){
    explicitTolerance = tol;
}
double QuickHull3D::getExplicitDistanceTolerance(){
    return explicitTolerance;
}

void QuickHull3D::addPointToFace(Vertex* vtx, Facet* facet){
    vtx->facet = facet;
    if (facet->outside == NULL){
        claimed->add(vtx);
    }
    else{
        claimed->insertBefore(vtx, facet->outside);
    }
    facet->outside = vtx;
}

void QuickHull3D::removePointFromFace(Vertex* vtx, Facet* facet){
    if (vtx == facet->outside){
        if (vtx->next != NULL && vtx->next->facet == facet)
            facet->outside = vtx->next;
        else
            facet->outside = NULL;
    }
    claimed->remove(vtx);
}

Vertex* QuickHull3D::removeAllPointsFromFace(Facet* facet){
    if (facet->outside != NULL){
        Vertex *end = facet->outside;
        while (end->next != NULL && end->next->facet == facet)
            end = end->next;
        claimed->removeAll(facet->outside, end);
        end->next = NULL;

        return facet->outside;
    }
    else{
        return NULL;
    }
}

HalfEdge* QuickHull3D::findHalfEdge(Vertex* tail, Vertex* head){
    int len = faces.size();
    for (int i = 0; i < len; i++){
        HalfEdge *he = faces[i]->findEdge(tail, head);
        if (he != NULL)
            return he;
    }
    return NULL;
}

void QuickHull3D::setHull(double* coords, int nump, vector<vector<int> > faceIndices, int numf){
    initBuffers(nump);
    setPoints(coords, nump);
    computeMaxAndMin();
    for (int i = 0; i < numf; i++){
        Facet *facet = Facet::create(pointBuffer, faceIndices[i]);
        HalfEdge *he = facet->he0;
        do{
            HalfEdge *heOpp = findHalfEdge(he->head(), he->tail());
            if (heOpp != NULL)
                he->setOpposite(heOpp);
            he = he->next;
        }while (he != facet->he0);
        faces.push_back(facet);
    }
}



void QuickHull3D::triangulate(){
    double minArea = 1000 * charLength * DOUBLEPRECISION;
    newFaces->clear();
    int len = faces.size();
    for (int i = 0; i < len; i++){
        Facet *facet = faces[i];
        if (facet->mark == VISIBLE){
            facet->triangulate(newFaces, minArea);
            if (newFaces->isEmpty()){
                qDebug("Empty");
            }
        }
    }
    for (Facet *facet = newFaces->first();
         facet != NULL; facet = facet->next)
        faces.push_back(facet);
}


void QuickHull3D::setPoints(double* coords, int nump){
    pointBuffer.clear();
    for (int i = 0; i < nump; i++){
        Vertex *vtx = new Vertex();
        vtx->pnt->set(coords[i*3], coords[i*3+1], coords[i*3+2]);
        vtx->index = i;
        pointBuffer.push_back(vtx);
    }
}

void QuickHull3D::setPoints(double** coords, int nump){
    pointBuffer.clear();
    for (int i = 0; i < nump; i++){
        Vertex *vtx = new Vertex();
        vtx->pnt->set(coords[i][0], coords[i][1], coords[i][2]);
        vtx->index = i;
        pointBuffer.push_back(vtx);
    }
}

void QuickHull3D::setPoints(vector<Point3D *> pnts, int nump){
    for (int i = 0; i < nump; i++){
        Vertex *vtx = pointBuffer[i];
        vtx->pnt->set(pnts[i]);
        vtx->index = 1;
    }
}

void QuickHull3D::computeMaxAndMin(){
    Vector3D *max = new Vector3D();
    Vector3D *min = new Vector3D();

    for (int i = 0; i < 3; i++){
        maxVtxs[i] = minVtxs[i] = pointBuffer[0];
    }
    max->set(pointBuffer[0]->pnt);
    min->set(pointBuffer[0]->pnt);

    for (int i = 1; i < numPoints; i++){
        Point3D *pnt = pointBuffer[i]->pnt;
        if (pnt->x > max->x){
            max->x = pnt->x;
            maxVtxs[0] = pointBuffer[i];
        }
        else if (pnt->x < min->x){
            min->x = pnt->x;
            minVtxs[0] = pointBuffer[i];
        }
        if (pnt->y > max->y){
            max->y = pnt->y;
            maxVtxs[1] = pointBuffer[i];
        }
        else if (pnt->y < min->y){
            min->y = pnt->y;
            minVtxs[1] = pointBuffer[i];
        }
        if (pnt->z > max->z){
            max->z = pnt->z;
            maxVtxs[2] = pointBuffer[i];
        }
        else if (pnt->z < min->z){
            min->z = pnt->z;
            minVtxs[2] = pointBuffer[i];
        }
    }

    charLength = MAX(max->x - min->x, max->y - min->y);
    charLength = MAX(charLength, max->z - min->z);

    if (explicitTolerance == AUTOMATIC_TOLERANCE){
        tolerance = 3 * DOUBLEPRECISION * (MAX(ABS(max->x), ABS(min->x))
                                     + MAX(ABS(max->y), ABS(min->y))
                                     + MAX(ABS(max->z), ABS(min->z)));
    }
    else
        tolerance = explicitTolerance;
}

void QuickHull3D::createInitialSimplex(){
    double max = 0;
    int imax = 0;

    for (int i = 0; i < 3; i++){
        double diff = maxVtxs[i]->pnt->get(i) - minVtxs[i]->pnt->get(i);
        if (diff > max){
            max = diff;
            imax = i;
        }
    }

    if (max <= tolerance){
        qDebug("Coincident input");
        return;
    }

    vector<Vertex*> vtx;
    vtx.push_back(maxVtxs[imax]);
    vtx.push_back(minVtxs[imax]);
    Vector3D *u01 = new Vector3D();
    Vector3D *diff02 = new Vector3D();
    Vector3D *nrml = new Vector3D();
    Vector3D *xprod = new Vector3D();
    double maxSqr = 0;
    u01->sub(*(vtx[1]->pnt), *(vtx[0]->pnt));
    u01->normalize();

    for (int i = 0; i < numPoints; i++){
        diff02->sub(*(pointBuffer[i]->pnt), *(vtx[0]->pnt));
        xprod->cross(*(u01), *(diff02));
        double lenSqr = xprod->normSquared();
        if (lenSqr > maxSqr && pointBuffer[i] != vtx[0]
                && pointBuffer[i] != vtx[1]){
            maxSqr = lenSqr;
            if (vtx.size() <= 2)
                vtx.push_back(pointBuffer[i]);
            else
                vtx[2] = pointBuffer[i];
            nrml->set(xprod);
        }
    }

    if (sqrt(maxSqr) <= 100 * tolerance){
        qDebug("Colinear input");
        return;
    }

    nrml->normalize();

    double maxDist = 0;
    double d0 = vtx[2]->pnt->dot(*(nrml));
    for (int i = 0; i < numPoints; i++){
        double dist = ABS(pointBuffer[i]->pnt->dot(*(nrml)) - d0);
        if (dist > maxDist && pointBuffer[i] != vtx[0]
                && pointBuffer[i] != vtx[1] && pointBuffer[i] != vtx[2]){
            maxDist = dist;
            if (vtx.size() <= 3)
                vtx.push_back(pointBuffer[i]);
            else
                vtx[3] = pointBuffer[i];
        }
    }

    if (ABS(maxDist) <= 100 * tolerance){
        qDebug("Coplanar input");
        return;
    }

    vector<Facet *> tris;

    if (vtx[3]->pnt->dot(*(nrml)) - d0 < 0){
        tris.push_back(Facet::createTriangle(vtx[0], vtx[1], vtx[2], 0));
        tris.push_back(Facet::createTriangle(vtx[3], vtx[1], vtx[0], 0));
        tris.push_back(Facet::createTriangle(vtx[3], vtx[2], vtx[1], 0));
        tris.push_back(Facet::createTriangle(vtx[3], vtx[0], vtx[2], 0));
        for (int i = 0; i < 3; i++){
            int k = (i+1)%3;
            tris[i+1]->getEdge(1)->setOpposite(tris[k+1]->getEdge(0));
            tris[i+1]->getEdge(2)->setOpposite(tris[0]->getEdge(k));
        }
    }
    else{
        tris.push_back(Facet::createTriangle(vtx[0], vtx[2], vtx[1], 0));
        tris.push_back(Facet::createTriangle(vtx[3], vtx[0], vtx[1], 0));
        tris.push_back(Facet::createTriangle(vtx[3], vtx[1], vtx[2], 0));
        tris.push_back(Facet::createTriangle(vtx[3], vtx[2], vtx[0], 0));
        for (int i = 0; i < 3; i++){
            int k = (i+1)%3;
            tris[i+1]->getEdge(0)->setOpposite(tris[k+1]->getEdge(1));
            tris[i+1]->getEdge(2)->setOpposite(tris[0]->getEdge((3-i)%3));
        }
    }

    for (int i = 0; i < 4; i++)
        faces.push_back(tris[i]);

    for (int i = 0; i < numPoints; i++){
        Vertex *vtemp = pointBuffer[i];
        if (vtemp == vtx[0] || vtemp == vtx[1]
                || vtemp == vtx[2] || vtemp == vtx[3])
            continue;
        maxDist = tolerance;
        Facet *maxFacet = NULL;

        for (int k = 0; k < 4; k++){
            double dist = tris[k]->distanceToPlane(vtemp->pnt);
            if (dist > maxDist){
                maxFacet = tris[k];
                maxDist = dist;
            }
        }

        if (maxFacet != NULL)
            addPointToFace(vtemp, maxFacet);

    }
}

int QuickHull3D::getNumVertices(){
    return numVertices;
}

vector<Point3D *> QuickHull3D::getVertices(){
    vector<Point3D *> vtxs;
    for (int i = 0; i < numVertices; i++)
        vtxs.push_back(pointBuffer[vertexPointIndices[i]]->pnt);
    return vtxs;
}

int QuickHull3D::getNumFaces(){
    return faces.size();
}

vector<vector<int> > QuickHull3D::getFaces(){
    return getFaces(0);
}

vector<vector <int> > QuickHull3D::getFaces(int indexFlags){
    vector<vector<int> > allFaces;
    bool ccw = ((indexFlags & CLOCKWISE) == 0);
    bool indexedFromOne = ((indexFlags & INDEXED_FROM_ONE) != 0);
    bool pointRelative = ((indexFlags & POINT_RELATIVE) != 0);
    int facesize = faces.size();

    for (int i = 0; i < facesize; i++){
        vector<int> vti;
        HalfEdge *hedge = faces[i]->he0;

        do{
            int idx = hedge->head()->index;

            if (pointRelative)
                idx = vertexPointIndices[idx];
            if (indexedFromOne)
                idx++;

            vti.push_back(idx);

            hedge = (ccw ? hedge->next : hedge->prev);

        }while (hedge != faces[i]->he0);
        allFaces.push_back(vti);
    }

    return allFaces;
}

void QuickHull3D::resolvedUnclaimedPoints(FaceList *newFaces){
    Vertex *vtxNext = unclaimed->first();
    for (Vertex *vtx = vtxNext; vtx != NULL; vtx = vtxNext){
        vtxNext = vtx->next;
        double maxDist = tolerance;
        Facet *maxFacet = NULL;
        for (Facet *ftmp = newFaces->first(); ftmp != NULL; ftmp = ftmp->next){
            if (ftmp->mark == VISIBLE){
                double dist = ftmp->distanceToPlane(vtx->pnt);
                if (dist > maxDist){
                    maxDist = dist;
                    maxFacet = ftmp;
                }
                if (maxDist > 1000 * tolerance)
                    break;
            }
        }
        if (maxFacet !=NULL)
            addPointToFace(vtx, maxFacet);
    }
}

void QuickHull3D::deleteFacePoints(Facet *facet, Facet *absorbingFace){
    Vertex *faceVtxs = removeAllPointsFromFace(facet);

    if (faceVtxs != NULL){
        if (absorbingFace == NULL)
            unclaimed->addAll(faceVtxs);
        else{

            Vertex *vtxNext = faceVtxs;
            for (Vertex *vtx = vtxNext; vtx != NULL; vtx = vtxNext){
                vtxNext = vtx->next;
                double dist = absorbingFace->distanceToPlane(vtx->pnt);
                if (dist > tolerance)
                    addPointToFace(vtx, absorbingFace);
                else
                    unclaimed->add(vtx);
            }
        }
    }
}

double QuickHull3D::oppFaceDistance(HalfEdge *he){
    return he->facet->distanceToPlane(he->opposite->facet->getCentroid());
}

bool QuickHull3D::doAdjacentMerge(Facet *facet, int mergeType){
    HalfEdge *hedge = facet->he0;
    bool convex = true;

    do{
        Facet *oppFace = hedge->oppositeFace();
        bool merge = false;

        if (mergeType == NONCONVEX){
            if (oppFaceDistance(hedge) > -tolerance ||
                    oppFaceDistance(hedge->opposite) > -tolerance)
                merge = true;
        }
        else{
            if (facet->area > oppFace->area){
                if (oppFaceDistance(hedge) > -tolerance)
                    merge = true;
                else if (oppFaceDistance(hedge->opposite) > -tolerance)
                    convex = true;
            }
            else{
                if (oppFaceDistance(hedge->opposite) > -tolerance)
                    merge = true;
                else if (oppFaceDistance(hedge) > -tolerance)
                    convex = false;
            }
        }

        if (merge){
            int numd = facet->mergeAdjacentFace(hedge, &discardedFaces);
            for (int i = 0; i < numd; i++)
                deleteFacePoints(discardedFaces[i], facet);
            return true;
        }

        hedge = hedge->next;
    }while (hedge != facet->he0);

    if (!convex)
        facet->mark = NON_CONVEX;
    return false;
}


void QuickHull3D::calculatedHorizon(Point3D *eyePnt, HalfEdge *edge0, Facet *facet, vector<HalfEdge *> *horizon){
    deleteFacePoints(facet, NULL);
    facet->mark = DELETED;

    HalfEdge *edge;
    int n = 0;
    if (edge0 == NULL){
        edge0 = facet->getEdge(0);
        edge = edge0;
    }
    else
        edge = edge0->getNext();

    do{
        Facet *oppFace = edge->oppositeFace();

        if (oppFace != NULL && oppFace->mark == VISIBLE){
            if (oppFace->distanceToPlane(eyePnt) > tolerance)
                calculatedHorizon(eyePnt, edge->getOpposite(), oppFace, horizon);
            else
                horizon->push_back(edge);
        }
        n++;
        edge = edge->getNext();
    }while (edge != edge0);
}


HalfEdge* QuickHull3D::addAdjoiningFace(Vertex *eyeVtx, HalfEdge *he){
    Facet *facet = Facet::createTriangle(eyeVtx, he->tail(), he->head(), 0);
    faces.push_back(facet);
    facet->getEdge(-1)->setOpposite(he->getOpposite());
    return facet->getEdge(0);
}

void QuickHull3D::addNewFaces(FaceList *newFaces, Vertex *eyeVtx, vector<HalfEdge*> horizon){
    newFaces->clear();

    HalfEdge *hedgeSidePrev = NULL;
    HalfEdge *hedgeSideBegin = NULL;
    for (int i = 0; i < horizon.size(); i++){
        HalfEdge *horizonHe = horizon[i];
        HalfEdge *hedgeSide = addAdjoiningFace(eyeVtx,horizonHe);
        if (hedgeSidePrev != NULL)
            hedgeSide->next->setOpposite(hedgeSidePrev);
        else
            hedgeSideBegin = hedgeSide;
        newFaces->add(hedgeSide->getFace());
        hedgeSidePrev = hedgeSide;
    }

    if (hedgeSideBegin != NULL)
        hedgeSideBegin->next->setOpposite(hedgeSidePrev);
}

Vertex* QuickHull3D::nextPointToAdd(){
    if (!(claimed->isEmpty())){
        Facet *eyeFace = claimed->first()->facet;
        Vertex *eyeVtx = NULL;
        double maxDist = 0;
        for (Vertex *vtx = eyeFace->outside; vtx != NULL && vtx->facet == eyeFace; vtx = vtx->next){
            double dist = eyeFace->distanceToPlane(vtx->pnt);
            if (dist > maxDist){
                maxDist = dist;
                eyeVtx = vtx;
            }
        }
        return eyeVtx;
    }

    return NULL;
}

void QuickHull3D::addPointToHull(Vertex *eyeVtx){
    horizon.clear();
    unclaimed->clear();
    removePointFromFace(eyeVtx,eyeVtx->facet);
    calculatedHorizon(eyeVtx->pnt, NULL, eyeVtx->facet, &horizon);
    newFaces->clear();
    addNewFaces(newFaces, eyeVtx, horizon);

    for (Facet *facet = newFaces->first(); facet != NULL; facet = facet->next){
        if (facet->mark == VISIBLE){
            while (doAdjacentMerge(facet, NONCONVEX_WRT_LARGER_FACE))
                ;
        }
    }

    for (Facet *facet = newFaces->first(); facet != NULL; facet = facet->next){
        if (facet->mark == NON_CONVEX){
            facet->mark = VISIBLE;
            while (doAdjacentMerge(facet, NONCONVEX))
                ;
        }
    }
    resolvedUnclaimedPoints(newFaces);
}

void QuickHull3D::buildHull(){
    int cnt = 0;
    Vertex *eyeVtx;
    computeMaxAndMin();
    createInitialSimplex();

    while ((eyeVtx = nextPointToAdd()) != NULL){
        addPointToHull(eyeVtx);
        cnt++;
    }

    reindexFacesAndVertices();
}

void QuickHull3D::markFaceVertices(Facet *facet, int mark){
    HalfEdge *he0 = facet->getFirstEdge();
    HalfEdge *he = he0;
    do {
        he->head()->index = mark;
        he = he->next;
    }while (he != he0);
}

void QuickHull3D::reindexFacesAndVertices(){
    for (int i = 0; i < numPoints; i++)
        pointBuffer[i]->index = -1;

    numFaces = 0;
    for (int i = 0; i < faces.size(); i++){
        Facet *facet = faces[i];
        if (facet->mark != VISIBLE){
            faces.erase(faces.begin()+i);
            i--;
        }
        else{
            markFaceVertices(facet, 0);
            numFaces++;
        }
    }

    numVertices = 0;
    vertexPointIndices.clear();
    for (int i = 0; i < numPoints; i++){
        Vertex *vtx = pointBuffer[i];
        if (vtx->index == 0){
            vertexPointIndices.push_back(i);
            vtx->index = numVertices++;
        }
    }
}

bool QuickHull3D::checkFaceConvexity(Facet *facet, double tol){
    double dist;
    HalfEdge *he = facet->he0;
    do{
        facet->checkConsistency();
        dist = oppFaceDistance(he);
        if (dist > tol)
            return false;

        dist = oppFaceDistance(he->opposite);
        if (dist > tol)
            return false;

        if (he->next->oppositeFace() == he->oppositeFace())
            return false;

        he = he->next;
    }while (he != facet->he0);
    return true;
}

bool QuickHull3D::checkFaces(double tol){
    bool convex = true;
    for (int i = 0; i < faces.size(); i++){
        Facet *facet = faces[i];
        if (facet->mark == VISIBLE){
            if (!checkFaceConvexity(facet, tol))
                convex = false;
        }
    }
    return convex;
}

bool QuickHull3D::check(){
    return check(getDistanceTolerance());
}

bool QuickHull3D::check(double tol){
    double dist;
    double pointTol = 10 * tol;

    if (!checkFaces(tolerance))
        return false;

    for (int i = 0; i < numPoints; i++){
        Point3D *pnt = pointBuffer[i]->pnt;
        for (int i = 0; i < faces.size(); i++){
            Facet *facet = faces[i];
            if (facet->mark == VISIBLE){
                dist = facet->distanceToPlane(pnt);
                if (dist > pointTol)
                    return false;
            }
        }
    }
    return true;
}



