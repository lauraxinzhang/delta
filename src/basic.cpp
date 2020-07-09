#include <cstdio>
#include <time.h> // for seeding the random engine.
#include <random>
#include <string>
#include <fstream>
#include <iostream>
#include <stdlib.h>

#include "Mesh.hpp"

//-------------- vector utilities -----------------------------
std::vector<double> randArray(double min, double max, int num)
{
    std::vector<double> out(num);
    std::default_random_engine generator(int(time(NULL)));
    // std::uniform_real_distribution<double> distribution(min,max);
    double mean = (min + max) / 2;
    double sigma = (mean - min)/2;
    std::normal_distribution<double> distribution(mean, sigma);
    for(int i = 0; i<num; i++){
        double val(min - 2);
        while (val < min || val > max){
            val = distribution(generator);
        }
        out[i] = val;
    }
    return out;
}

std::vector<double> linspace(double start, double end, int num)
{
    std::vector<double> out(num);
    double d = (end - start) / (num - 1);
    for(int i = 0; i<num; i++){
        out[i] = start + i * d;
    }
    return out;
}

std::vector<double> meshgrid(std::vector<double>& rr, std::vector<double>& zz)
{
    int nr = rr.size();
    int nz = zz.size();

    std::vector<double> out(nr * nz * 2);
    for(int i = 0; i < nr; i++){
        for(int j = 0; j < nz; j++){
            int index = (nz * i + j) * 2;
            out[index]    = rr[i];
            out[index +1] = zz[j];
        }
    }
    return out;
}

int main() {

    /* x0, y0, x1, y1, ... */
//--------- option 1: a small test grid
    std::vector<double> coords = {-1, 1, 1, 1, 1, -1, -1, -1, 0, 0, -1, 0};

    
//--------- option 2: a mesh grid (uncomment to test)
    
//    std::vector<double> rr = linspace(0, 3, 20);
//    std::vector<double> zz = linspace(0, 4, 25);
//
//    std::vector<double> rr = randArray(0, 3, 2000);
//    std::vector<double> zz = randArray(0, 4, 1000);
//
//    std::vector<double> coords = meshgrid(rr, zz);

//--------- option 3: random points (uncomment to test)
//    std::vector<double> coords = randArray(0, 4, 2000);


//    //triangulation happens here
//    delaunator::Delaunator d(coords);

    VecDoub val = {0, 1, 2, 3, 4, 5}; // to use with the small test grid option #1
    Mesh mesh(coords, val);
    std::cout <<"size of mesh: " << mesh.size() << std::endl;
    
    // print triangulation to file
    mesh.printTriag("triangles.txt");
    
    // search for this point
    MeshPoint p(1, -1);
    // initial guess triangle for search
    size_t start = 0;
    std::vector<size_t> tout = mesh.search(p, start);

    // tout now contains the full search path
    std::vector<MeshPoint> t_coords = mesh.coordsOfTriag(tout.back());
    // print the vertices of the triangle the search point is in
    std::cout << "Found in triangle " << tout.back() << std::endl;
    for (int i=0; i< t_coords.size(); i++){
        std::cout << t_coords[i].x_ << "," << t_coords[i].y_ << std::endl;
    }
    
    // print search path to file
    FILE * hist;
    hist = fopen("search.txt", "w");
    for(int i=0; i<tout.size(); i++){
        fprintf(hist, "%i\n", tout[i]);
    }
    fclose(hist);

    std::cout << "Thanks for using! Bye!" << std::endl;
}
