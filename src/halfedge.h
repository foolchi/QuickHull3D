#ifndef HALFEDGE_H
#define HALFEDGE_H
/**
 * @file halfedge.h
 * @brief Represent the half-edge of the face in a counter-clockwise direction
 **/

#include "facet.h"
#include "vertex.h"

/**
 * @brief Half-edge
 */

class HalfEdge{

public:
    HalfEdge *prev, *next, *opposite; /*!< Previous, next and opposite half-edge */
    Facet *facet;
    Vertex *vertex;

    /**
     * @brief Constructor
     */
    HalfEdge();

    /**
     * @brief Constructor
     * @param v head vertex
     * @param f left-hand triangular face
     */
    HalfEdge(Vertex *v, Facet *f);

    /**
     * @brief Set the vertex and face
     * @param v head vertex
     * @param f left-hand triangular face
     */
    void setVertexAndFacet(Vertex *v, Facet *f);

    /**
     * @brief Set the value of the next edge adjacent
     * (counter-clockwise) to this one within the triangle
     * @param edge next adjacent edge
     */
    void setNext(HalfEdge *edge);

    /**
     * @brief Get the value of the next edge adjacent
     * (counter-clockwise) to this one within the triangle.
     * @return Next adjacent edge
     */
    HalfEdge* getNext();

    /**
     * @brief Set the value of the previous edge adjacent
     * (clockwise) to this one within the triangle.
     * @param edge previous adjacent edge
     */
    void setPrev(HalfEdge *edge);

    /**
     * @brief Set the value of the previous edge adjacent
     * (clockwise) to this one within the triangle.
     * @return Previous adjacent edge
     */
    HalfEdge* getPrev();

    /**
     * @brief Return the triangular face located to the left ot the half-edge
     * @return Left-hand triangular face
     */
    Facet* getFace();

    /**
     * @brief Return the half-edge opposite to this half-edge
     * @return Opposite half-edge
     */
    HalfEdge* getOpposite();

    /**
     * @brief Set the half-edge opposite to this half-edge
     * @param edge opposite half-edge
     */
    void setOpposite(HalfEdge* edge);

    /**
     * @brief Return the head vertex associated with this half-edge
     * @return Head vertex
     */
    Vertex* head();

    /**
     * @brief Return the tail vertex associated with this half-edge
     * @return Tail vertex
     */
    Vertex* tail();

    /**
     * @brief Return the opposite triangular face
     * @return Opposite triangular face
     */
    Facet* oppositeFace();

    /**
     * @brief Return the length of this half-edge
     * @return Length of half-edge
     */
    double length();

    /**
     * @brief Return the squared length of this half-edge
     * @return Squared length
     */
    double lengthSquared();

};

#endif // HALFEDGE_H
