//  Mesh.cpp
//  delta_xcode
//
//  Created by Xin Zhang on 6/30/20.
//  Copyright © 2020 Xin Zhang. All rights reserved.
//

#include "Mesh.hpp"

LineSeg::LineSeg(MeshPoint& pa, MeshPoint& pb)
        :pa_(pa), pb_(pb) // using copy constructor
{
    // nothing else to do
}

bool LineSeg::parallel(LineSeg &line_b)
{
//    MeshPoint diff_a, diff_b;
    MeshPoint diff_a = pa_ - pb_;
    MeshPoint diff_b = line_b.pa_ - line_b.pb_;
    double result = diff_a.cross(diff_b);
    return (abs(result) < 1e-15);
}

double LineSeg::signedArea(MeshPoint& pc)
{
    MeshPoint diff_ba, diff_ca;
    diff_ba = pb_ - pa_;
    diff_ca = pc - pa_;
    return diff_ba.cross(diff_ca) / 2;
}

MeshPoint LineSeg::intersect(LineSeg &line_b)
{
    bool par = parallel(line_b);
    MeshPoint result;
    if (par){
        double nan = std::nan("0");
        MeshPoint rtn(nan, nan);
        result = rtn;
    }
    else{
//        MeshPoint diff_a, diff_b;
        MeshPoint diff_a = pb_ - pa_;
        MeshPoint diff_b = line_b.pa_ - line_b.pb_;
        double denom = 1 / diff_a.cross(diff_b);
        
//        MeshPoint diff_heads;
        MeshPoint diff_heads = pa_ - line_b.pa_;
        double s = diff_b.cross(diff_heads) * denom;
        double t = diff_heads.cross(diff_a) * denom;
        MeshPoint rtn(s, t);
        result = rtn;
    }
    return result;
}

bool LineSeg::isCross(LineSeg &line_b)
{
    MeshPoint inter = intersect(line_b);
    bool head = inter.x_>0 && inter.x_ < 1;
    bool tail = inter.y_>0 && inter.y_ < 1;
    return head && tail;
}


Mesh::Mesh(VecDoub& coords, VecDoub& val)
    :d_(coords), coords_(coords), val_(val)
{
    // Delaunator is constructed in colon initialization
    if (val_.size() != coords_.size() /2){
        try {
            throw ExitException(1);
        } catch (ExitException& e) {
            std::cerr << "Found " << val.size() << " values for ";
            std::cerr << coords_.size() /2 << " pairs of coordinates." << std::endl;
            std::cout << e.what() << std::endl;
        }
    }
}

size_t Mesh::size()
{
    return val_.size();
}

size_t Mesh::numTriag()
{
    return d_.triangles.size()/3;
}

std::vector<size_t> Mesh::edgesOfTriag(size_t t)
{
    try {
        if (t > d_.triangles.size()){
            
            std::cout << t << " huh>>>" << std::endl;
            throw ExitException(4);
        }
    } catch (ExitException& e) {
        e.what();
    }
    t = t - (t % 3); // make sure we're at the beginning of the triangle
    std::vector<size_t> out {t, t +1, t + 2};
    return out;
}

std::vector<size_t> Mesh::pointsOfTriag(size_t t)
{
    std::vector<size_t> out(6);
    t = t - (t % 3); // make sure we're at the beginning of the triangle
    std::vector<size_t> edges = edgesOfTriag(t);
    for (int i = 0; i < 3; i++){
//        size_t coord_id = d_.triangles[t + i];
        size_t coord_id = d_.triangles.at(edges[i]); // safe retrieval
        out[2 * i]     = 2 * coord_id;     // x coordinate
        out[2 * i + 1] = 2 * coord_id + 1; // y coordinate
    }
    return out;
}

size_t Mesh::triagOfEdge(size_t e)
{
    try {
        if (e >= d_.triangles.size()){
            throw ExitException(5);
        }
    } catch (ExitException& e) {
        e.what();
    }
    return e - (e % 3);
//    return floor(e/3);
}


std::vector<MeshPoint> Mesh::coordsOfTriag(size_t t)
{
    std::vector<MeshPoint> out(3);
    t = t - (t % 3); // go back to the beginning of the triangle
    std::vector<size_t> indices = pointsOfTriag(t);
    for (int i = 0; i < 3; i++){
        out[i].x_ = d_.coords.at(indices[ 2 * i]);
        out[i].y_ = d_.coords.at(indices[ 2 * i + 1]);
    }
    return out;
}

LineSeg Mesh::edgeToLineSeg(size_t e)
{
    size_t start = d_.triangles[e];
    double a_x = d_.coords.at(2 * start);
    double a_y = d_.coords.at(2 * start + 1);
    
    size_t end = d_.halfedges.at(e);
    size_t coord_id;
//    size_t end;
    if (end == delaunator::INVALID_INDEX){ // there's no opposite triangle
        // look for the start of next half edge
        coord_id = d_.triangles.at((e % 3 == 2) ? e - 2 : e + 1);
    }
    else {
        coord_id = d_.triangles.at(end);
    }
    double b_x = d_.coords.at(2 * coord_id);
    double b_y = d_.coords.at(2 * coord_id + 1);
    
    MeshPoint pa(a_x, a_y);
    MeshPoint pb(b_x, b_y);
    LineSeg rtn(pa, pb);
    return rtn;
}

size_t Mesh::neighborTriag(size_t e)
{
    size_t opposite = d_.halfedges.at(e);
    size_t triag;
    if (opposite != -1){
        triag = triagOfEdge(opposite);
    } else {
        triag = -1; // neighboring triangle doesn't exist
    }
    return triag;
}


VecDoub Mesh::barycentric(MeshPoint point, size_t t)
{
//    assert(point.size() == 2); // make sure that point is a valid coord
    double x = point.x_;
    double y = point.y_;
    
    std::vector<MeshPoint> coords = coordsOfTriag(t);
    double x1 = coords[0].x_;
    double y1 = coords[0].y_;
    double x2 = coords[1].x_;
    double y2 = coords[1].y_;
    double x3 = coords[2].x_;
    double y3 = coords[2].y_;
    
    double inv_det = 1/ ( (x1 - x3) * (y2 - y3) - (x2 - x3) * (y1 - y3) );
    VecDoub out(3);
    out[0] = inv_det * ( (y2 - y3) * (x - x3) + (x3 - x2) * (y - y3) );
    out[1] = inv_det * ( (y3 - y1) * (x - x3) + (x1 - x3) * (y - y3) );
    out[2] = 1 - out[0] - out[1];
    
    return out;
}


bool Mesh::isInTriag(MeshPoint point, size_t t)
{
    VecDoub bar = barycentric(point, t);
    bool rtn = true;
    for (int i = 0; i < bar.size(); i++){
        rtn = rtn && (bar[i] >= 0) && (bar[i] <= 1);
    }
    return rtn;
}

MeshPoint Mesh::centroid(size_t t)
{
    std::vector<MeshPoint> coords = coordsOfTriag(t);
    double sumx(0), sumy(0);
    for (int i = 0; i < coords.size(); i++){
        sumx += coords[i].x_;
        sumy += coords[i].y_;
    }
    double xout = sumx/3;
    double yout = sumy/3;
    MeshPoint rtn(xout, yout);
    return rtn;
}

std::vector<size_t> Mesh::search(MeshPoint p, size_t init)
{
    std::vector<size_t> rtn;
    size_t t_now(init);
    rtn.push_back(t_now);
//    size_t t_next;
    while (!isInTriag(p, t_now)){
        std::cout << t_now << std::endl;
        size_t t_next = walk(p, t_now);
        t_now = t_next;
        rtn.push_back(t_now);
    }
    return rtn;
}

size_t Mesh::walk(MeshPoint p, size_t t_now)
{
    std::vector<size_t> edges_head = edgesOfTriag(t_now);
    
    size_t intersection(delaunator::INVALID_INDEX); // an invalid result to initialize
    for (int i=0; i< edges_head.size(); i++){
        size_t e = edges_head[i];
        LineSeg edge = edgeToLineSeg(e);
        MeshPoint center = centroid(t_now);
        LineSeg query(p, center);
        if (query.isCross(edge)){
            intersection = e;
        }
    }
    try {
        if (intersection == delaunator::INVALID_INDEX){
            // no intersection found
            throw ExitException(2);
        }
        size_t e_opposite = d_.halfedges[intersection];
        if (e_opposite == delaunator::INVALID_INDEX){
            // point is outside domain
            throw ExitException(3);
        }
        size_t t_next = triagOfEdge(e_opposite);
        // program success
        return t_next;
        
    } catch (ExitException& e) {
        std::cerr << e.what() << std::endl;
    }
    // function should terminate before it gets here
    return delaunator::INVALID_INDEX;
}

double Mesh::interp(MeshPoint p, size_t init)
{
    std::vector<size_t> triag = search(p, init);
    VecDoub bary = barycentric(p, triag.back());
    
    std::vector<size_t> points = pointsOfTriag(triag.back());
    double out(0);
    for (int i=0; i<3; i++){
        out += bary[i] * val_[points[i]];
    }
    return out;
}

void Mesh::printTriag(const char* fname){
    // print triangulation to file
    FILE * pFile;
    pFile = fopen (fname,"w");
    for(std::size_t i = 0; i < d_.triangles.size(); i+=3) {
        fprintf(pFile,
            // "[[%f, %f], [%f, %f], [%f, %f]]\n",
            "%f, %f\n %f, %f\n %f, %f\n",
            d_.coords[2 * d_.triangles[i]],        //tx0
            d_.coords[2 * d_.triangles[i] + 1],    //ty0
            d_.coords[2 * d_.triangles[i + 1]],    //tx1
            d_.coords[2 * d_.triangles[i + 1] + 1],//ty1
            d_.coords[2 * d_.triangles[i + 2]],    //tx2
            d_.coords[2 * d_.triangles[i + 2] + 1] //ty2
        );
    }
    fclose(pFile);
}
