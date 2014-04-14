#ifndef FACET_H
#define FACET_H
/**
 * @file facet.h
 * @brief Face for quickhull3D
 */

#include <QApplication>
#include <QtGlobal>
#include "vector3d.h"
#include "point3d.h"
#include <math.h>
#include <vector>

class Vertex;
class FaceList;
class HalfEdge;
using namespace std;

const int VISIBLE = 0x1;
const int NON_CONVEX = 0x2;
const int DELETED = 0x4;

class Facet{

private:
    Vector3D *normal; /*!< Vector normal for the face */
    Point3D *centroid; /*!< Center point of the face */

public:
    HalfEdge *he0;
    double area;
    double planeOffset;
    int index;
    int numVerts;
    int mark;
    Facet *next;
    Vertex *outside;
    /**
     * @brief Constructor
     */
    Facet();

    /**
     * @brief Calculate the center point
     * @param centroid Center point
     */
    void computeCentroid(Point3D *centroid);

    /**
     * @brief Calculate the vertor normal
     * @param normal Vector normal
     * @param minArea Area minimal
     */
    virtual void computeNormal(Vector3D *normal, double minArea);

    /**
     * @brief Calculate the vertor normal
     * @param normal Vector normal
     */
    virtual void computeNormal(Vector3D *normal);

    /**
     * @brief Calculate vector normal and center point
     */
    virtual void computeNormalAndCentroid();

    /**
     * @brief Calculate vector normal and center point
     * @param minArea Area minimal
     */
    virtual void computeNormalAndCentroid(double minArea);

    /**
     * @brief Create a triangle
     * @param v0 Vertex 0
     * @param v1 Vertex 1
     * @param v2 Vertex 2
     * @param minArea
     * @return
     */
    static Facet* createTriangle(Vertex *v0, Vertex *v1, Vertex *v2,
                                  double minArea);

    /**
     * @brief Create a face using a vector of vertex
     * @param vtxarray Vector of vertex
     * @param indices Indices of the vertexs
     * @return
     */
    static Facet* create(vector<Vertex*> vtxarray, vector<int> indices);

    /**
     * @brief Get the i-th half-edge in the face
     * @param i the half-edge index (0-2)
     * @return The half-edge
     */
    HalfEdge* getEdge(int i);

    /**
     * @brief Get first half-edge
     * @return The first half-edge
     */
    HalfEdge* getFirstEdge();

    /**
     * @brief Find the half-edge in the face which
     * @param vt tail point
     * @param vh head point
     * @return The half-edge, NULL if not found
     */
    HalfEdge* findEdge(Vertex *vt, Vertex *vh);

    /**
     * @brief Calculate the distance of one point to the face
     * @param p Point
     * @return Distance
     */
    double distanceToPlane(Point3D *p);

    /**
     * @brief Get the vector normal
     * @return The vector normal
     */
    Vector3D* getNormal();

    /**
     * @brief Get the center point
     * @return The center point
     */
    Point3D* getCentroid();

    /**
     * @brief Get the number of vertex in the face
     * @return The number of vertex
     */
    int numVertices();

    /**
     * @brief Get all the index of each vertex
     * @return Vector of index of the vertex
     */
    vector<int> getVertexIndices();

    /**
     * @brief Connect two half-edges
     * @param hedgePrev
     * @param hedge
     * @return
     */
    Facet* connectHalfEdges(HalfEdge *hedgePrev, HalfEdge *hedge);

    /**
     * @brief Check whether the face is legal
     */
    void checkConsistency();

    /**
     * @brief Merge the half-edges
     * @param hedgeAdj half-edges
     * @param discarded faces to be deleted
     * @return number of faces to be deleted
     */
    int mergeAdjacentFace(HalfEdge *hedgeAdj,
                          vector<Facet*> *discarded);

    /**
     * @brief Calculate the squared area of the triangle defined by the two half-edges
     * @param hedge0
     * @param hedge1
     * @return Squared area
     */
    double areaSquared(HalfEdge *hedge0, HalfEdge *hedge1);

    /**
     * @brief Create new faces
     * @param newFaces
     * @param minArea
     */
    void triangulate(FaceList *newFaces, double minArea);
};


#endif // FACET_H
