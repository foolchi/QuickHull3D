#ifndef QUICKHULL3D_H
#define QUICKHULL3D_H
/**
 * @file quickhull3d.h
 * @brief quickhull3d algorithme
 */

#include "vertex.h"
#include <vector>
#include "facelist.h"
#include "halfedge.h"
#include "vertexlist.h"
#include "facet.h"
#include "func.h"

using namespace std;

const int CLOCKWISE = 0x1;
const int INDEXED_FROM_ONE = 0x2;
const int INDEXED_FROM_ZERO = 0x4;
const int POINT_RELATIVE = 0x8;
const double AUTOMATIC_TOLERANCE = -1.0;
const int findIndex = -1;
const int NONCONVEX_WRT_LARGER_FACE = 1;
const int NONCONVEX = 2;

/**
 * @brief The QuickHull3D class
 */

class QuickHull3D
{

private:
    FaceList *newFaces;
    VertexList *unclaimed;
    VertexList *claimed;
    vector<Vertex*> maxVtxs;
    vector<Vertex*> minVtxs;
    double explicitTolerance;
    double tolerance;
    vector<Facet*> faces;

public:
    double charLength;
    vector<Vertex*> pointBuffer;
    vector<int> vertexPointIndices;
    vector<Facet*> discardedFaces;
    vector<HalfEdge*> horizon;
    int numVertices;
    int numFaces;
    int numPoints;

    /**
     * @brief Constructor
     */
    QuickHull3D();

    /**
     * @brief Constructor
     * @param coords coordinates of points
     * @param len number of coordinated (3*number of points)
     */
    QuickHull3D(double* coords, int len);

    /**
     * @brief Constructor
     * @param coords points
     * @param len number of points
     */
    QuickHull3D(double** coords, int len);

    /**
     * @brief Set points
     * @param coords coordinates of points
     * @param nump number of coordinated (3*number of points)
     */
    virtual void setPoints(double* coords, int nump);

    /**
     * @brief Set points
     * @param coords points
     * @param nump number of points
     */
    virtual void setPoints(double** coords, int nump);

    /**
     * @brief Build the convex
     * @param coords coordinates of points
     * @param nump number of coordinated (3*number of points)
     */

    /**
     * @brief Set points
     * @param pnts vector of point3d
     * @param nump number of points
     */
    virtual void setPoints(vector<Point3D *> pnts, int nump);

    /**
     * @brief Build the convex
     * @param coords coordinates of points
     * @param nump number of coordinated (3*number of points)
     */
    virtual void build(double *coords, int nump);

    /**
     * @brief Build the convex
     * @param coords points
     * @param nump number of points
     */
    virtual void build(double **coords, int nump);

    /**
     * @brief Return the distance tolerance
     * @return Distance tolerance
     */
    double getDistanceTolerance();


    void initBuffers(int nump);

    /**
     * @brief Set an explicit distance tolerance
     * @param tol explicit distance tolerance
     */
    void setExplicitDistanceTolerance(double tol);

    /**
     * @brief Get an explicit distance tolerance
     * @return Explicit distance tolerance
     */
    double getExplicitDistanceTolerance();

    /**
     * @brief Add a point to face
     * @param vtx point to be added
     * @param facet face
     */
    void addPointToFace(Vertex* vtx, Facet* facet);

    /**
     * @brief Remove a point from face
     * @param vtx point to be removed
     * @param facet face
     */
    void removePointFromFace(Vertex* vtx, Facet* facet);

    /**
     * @brief Remove all the points from face
     * @param facet face
     * @return first vertex (outside)
     */
    Vertex* removeAllPointsFromFace(Facet* facet);

    /**
     * @brief Find the half-edge between two vertices
     * @param tail tail vertex
     * @param head head vertex
     * @return Half-edge, NULL if not found
     */
    HalfEdge* findHalfEdge(Vertex* tail, Vertex* head);

    void setHull(double* coords, int nump, vector<vector<int> > faceIndices, int numf);

    /**
     * @brief Transform a non-trangular hull face to triangle
     */
    void triangulate();

    /**
     * @brief Calculate the max and min coordiantes
     */
    void computeMaxAndMin();

    /**
     * @brief Creates the initial simplex from which the hull will be built
     */
    void createInitialSimplex();

    /**
     * @brief Get the number of vertices
     * @return The number of vertices
     */
    int getNumVertices();

    /**
     * @brief Get all the vertices
     * @return Vector of vertices
     */
    vector<Point3D *> getVertices();

    /**
     * @brief Get the number of faces
     * @return The number of faces
     */
    int getNumFaces();

    /**
     * @brief Get all the faces
     * @return Vector of faces
     */
    virtual vector<vector <int> > getFaces();

    /**
     * @brief Get all the faces
     * @param Specifies index characteristics (0 results in the default)
     * @return Vector of faces
     */
    virtual vector<vector <int> > getFaces(int indexFlags);

    void resolvedUnclaimedPoints(FaceList *newFaces);

    /**
     * @brief Delete all the points from the face
     * @param facet face whose points to be deleted
     * @param absorbingFace face that will add the deleted points
     */
    void deleteFacePoints(Facet *facet, Facet *absorbingFace);

    /**
     * @brief Get the distance from the opposite face to this face
     * @param he half-edge
     * @return The distance of the center point of the opposite face
     * to the face
     */
    double oppFaceDistance(HalfEdge *he);

    /**
     * @brief Merge the face
     * @param facet face to be merged
     * @param mergeType NONCONVEX, NONCONVEX_WRT_LARGER_FACE
     * @return True if succeed, false if not
     */
    bool doAdjacentMerge(Facet *facet, int mergeType);

    /**
     * @brief Calculate the horizontal
     * @param eyePnt eyepoint
     * @param edge0 initial half-edge
     * @param facet initial face
     * @param horizon the horizontal half-edge to be stored
     */
    void calculatedHorizon(Point3D *eyePnt, HalfEdge *edge0,
                           Facet *facet, vector<HalfEdge *> *horizon);


    HalfEdge* addAdjoiningFace(Vertex *eyeVtx, HalfEdge *he);
    void addNewFaces(FaceList *newFaces, Vertex *eyeVtx,
                     vector<HalfEdge*> horizon);
    Vertex* nextPointToAdd();
    void addPointToHull(Vertex *eyeVtx);
    void buildHull();
    void markFaceVertices(Facet *facet, int mark);
    void reindexFacesAndVertices();
    bool checkFaceConvexity(Facet *facet, double tol);
    bool checkFaces(double tol);
    virtual bool check();
    virtual bool check(double tol);

};

#endif // QUICKHULL3D_H
