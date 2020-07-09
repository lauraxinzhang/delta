//  Mesh.hpp
//  delta_xcode
//
//  Created by Xin Zhang on 6/30/20.
//  Copyright © 2020 Xin Zhang. All rights reserved.
//

#ifndef mesh_h
#define mesh_h

#include "delaunator.hpp"
#include "Containers.hpp"
#include <cmath>
#include <exception>


struct MeshPoint
{
    double x_;
    double y_;
    
    inline MeshPoint():x_(0), y_(0) {}
    
    inline MeshPoint(const double& x, const double& y):x_(x), y_(y){}
    
    inline MeshPoint(const MeshPoint& p): x_(p.x_), y_(p.y_) {}
    
//    inline MeshPoint& operator=(MeshPoint p)
//    {
//        x_ = p.x_;
//        y_ = p.y_;
//        return *this;
//    }
    
    inline bool operator==(MeshPoint& p)
    {
        return p.x_ == x_ && p.y_ == y_;
    }
    
    inline MeshPoint operator-(MeshPoint& pb)
    {
        double x = x_ - pb.x_;
        double y = y_ - pb.y_;
        MeshPoint p(x, y);
        return p;
    }
    
    inline double cross(MeshPoint& pb)
    {
        double rtn = x_ * pb.y_ - pb.x_ * y_;
        return rtn;
    }
};

struct LineSeg
{
    MeshPoint pa_;
    MeshPoint pb_;
    
    LineSeg(MeshPoint& pa, MeshPoint& pb);
    
    bool parallel(LineSeg& line_b);
    
    /**
     *\brief Value of signed area of the triangle formed by the current linesegment and point c
     *\param pc the 3rd MeshPoint to form triangle with
     *\note Return value is positive is pc is "left-of" line segment
     */
    double signedArea(MeshPoint& pc);
    
    /**
     *\brief Computes the intersection of two lines.
     *\param line_b Query line segment.
     *\return MeshPoint(s, t) of the intersection, in the basis of the line segments. See nr3 ch21.4.
     */
    MeshPoint intersect(LineSeg& line_b);
    
    /**
     *\brief Whether the two line SEGMENTS intersect
     *\param line_b Query line segment
     *\return true if the computed intersect lies *on* the line segment, excluding end points.
     */
    bool isCross(LineSeg& line_b);
};

class Mesh
{
public:
    /**
     *\brief Constructor for Mesh class. Constructs the triangulation from input coordinates
     *\param coords Set of coordinates for input points, as one vector {x1, y1, x2, y2, ...}
     */
    Mesh(VecDoub& coords, VecDoub& val);
    
    /**
     *\brief Get number of coordinate pairs
     */
    size_t size();
    
    /**
     *\brief Get the number of total triangles
     */
    size_t numTriag();
    
    /**
     *\brief  Find the half edges coresponding to the input triangle
     *\param  t index of triangle
     *\return Indices of the edges of the t-th triangle
     */
    std::vector<size_t> edgesOfTriag(size_t t);
    
    /**
     *\brief Find the indices to the points of triangles in the coord list
     *\param t index of triangle
     *\return indices of points
     */
    std::vector<size_t> pointsOfTriag(size_t t);
    
    /**
     *\brief Find the triangle that the given half edge is in
     *\param e index of the half edge
     *\return Index of the resulting triangle (index t that the triangle starts in)
     */
    size_t triagOfEdge(size_t e);
    
    /**
     *\brief Give the coordinates of the vertices of triangle
     *\param t index of triangle
     *\return coordinates of vertices
     */
    std::vector<MeshPoint> coordsOfTriag(size_t t);
    
    /**
     *\brief Give the coordinates of the end points of an half edge (head, tail)
     *\param e index of halfedge
     *\return coordinates of endpoints
     */
    LineSeg edgeToLineSeg(size_t e);
    
    /**
     *\brief Find the opposite halfedge of the given edge, and then the triangle it belongs to
     *\param e index of the half edge in the current triangle.
     *\return index of the neighbor triangle; -1 if there's no neighbor
     */
    size_t neighborTriag(size_t e);
    
    
    /**
     *\brief Calculates the barycentric coordinate of point in triangle
     *\param point coordinate {x, y} of query point
     *\param t          index of triangle
     *\return Barycentric coordinate {lambda1, lambda2, lambda3}; expression from wikipedia
     */
    VecDoub barycentric(MeshPoint point, size_t t);
    
    /**
     *\brief Whether a point is in the current triangle
     *\param point coordinate {x, y} of the query point
     *\param t          index of the current triangle
     *\details Calculates the barycentric coordinate. If all coordinates are between [0, 1],
     *           the point is in the triangle.
     */
    bool isInTriag(MeshPoint point, size_t t);
    
    /**
     *\brief Finds the centroid of the trianglge
     *\param t Index of the triangle
     *\note  This is a point that is gauranteed to be *inside* the triangle. Its barycentric coordinate
     *          should always be {1/3, 1/3, 1/3}
     */
    MeshPoint centroid(size_t t);
    
    /**
     *\brief Look for which triangle the point is in by "walking" through the triangulation
     *\param p The point to search for
     *\param init Index of the triangle to start with
     */
    std::vector<size_t> search(MeshPoint p, size_t init);
    
    /**
     *\brief Walks to the next closest triangle
     *\param p MeshPoint to locate
     *\param t_now Index of current triangle
     *\return index of adjacent triangle to walk to
     */
    size_t walk(MeshPoint p, size_t t_now);
    
    /**
     *\brief Linear interpolation on triangular grid
     *\param p        Query point for interpolation
     *\param init Index of the initial angle to start searching in
     *\details Search for which triangle the point is in, then find the barycentric coordinate of the point. The interpolated value is then sum(lambda_i * val_i).
     */
    double interp(MeshPoint p, size_t init);
    
private:
    delaunator::Delaunator d_;
    VecDoub coords_;
    VecDoub val_; //length is half of the length of coords; Value on each grid point
};

/**
 *\brief Housekeeping. Allows for exiting without memory leaks
 */
struct ExitException : public std::exception {
    int code_; ///< Exit code
    ExitException(int code): code_(code) {} // trivial constructor
    
    const char * what () const throw () {
        switch (code_){
            case 0: std::cerr << std::endl;
                exit(0); //normal exit
            case 1:
                std::cerr << "coordinates and function values dimension mismatch." << std::endl;
                std::cerr << "Exiting." << std::endl;
                exit(1);
            case 2:
                std::cerr << "No intersection found with current triangle." << std::endl;
                std::cerr << "Exiting." << std::endl;
                exit(2);
            case 3:
                std::cerr << "Search target point is outside domain. Exiting...";
                std::cerr << std::endl;
                exit(3);
            case 4:
                std::cerr << "Triangle index out of range." << std::endl;
                std::cerr << "Called by Mesh::edgesOfTriag" << std::endl;
                exit(4);
            case 5:
                std::cerr << "Edge index out of range." << std::endl;
                std::cerr << "Called by Mesh::triagOfEdge" << std::endl;
                exit(5);
        }
        return "Uncaught exceptions";
   }
};




#endif /* mesh_h */
