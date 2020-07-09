#include <cstdio>
#include <time.h> // for seeding the random engine.
#include <random>
#include <string>
#include <fstream>
#include <iostream>

#include "delaunator.hpp"
#include "mesh.hpp"
#include "Containers.hpp"

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

// Does this show up in original file??
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

std::vector<double> read_limiter(std::string& filename)
{
    std::ifstream input;
    input.open(filename);
    std::vector<double> rtn;
    
    if (!input.is_open()){
        std::cerr << "cannot open file" << std::endl;
//        return rtn;
    } else {
        double in;
        while (input >> in){
            rtn.push_back(in);
        }
    }
    input.close();
    return rtn;
}

int main() {
    /* x0, y0, x1, y1, ... */
//     std::vector<double> coords = {-1, 1, 1, 1, 1, -1, -1, -1, 0, 0, -1, 0};
    
    // std::vector<double> rr = linspace(0, 3, 20);
    // std::vector<double> zz = linspace(0, 4, 25);

//    std::vector<double> rr = randArray(0, 3, 2000);
//    std::vector<double> zz = randArray(0, 4, 1000);
//
//    // std::vector<double> coords = meshgrid(rr, zz);
//
    std::vector<double> coords = randArray(0, 4, 2000);
//    std::string fname("limit_flat.txt");
//    std::vector<double> coords = read_limiter(fname);


    //triangulation happens here
    delaunator::Delaunator d(coords);

    FILE * pFile;
    pFile = fopen ("triangles.txt","w");
    for(std::size_t i = 0; i < d.triangles.size(); i+=3) {
        fprintf(pFile,
            // "[[%f, %f], [%f, %f], [%f, %f]]\n",
            "%f, %f\n %f, %f\n %f, %f\n",
            d.coords[2 * d.triangles[i]],        //tx0
            d.coords[2 * d.triangles[i] + 1],    //ty0
            d.coords[2 * d.triangles[i + 1]],    //tx1
            d.coords[2 * d.triangles[i + 1] + 1],//ty1
            d.coords[2 * d.triangles[i + 2]],    //tx2
            d.coords[2 * d.triangles[i + 2] + 1] //ty2
        );
    }
    fclose(pFile);
    
    
    
    VecDoub val = {0, 1, 2, 3};
    Mesh mesh(coords, val);
    MeshPoint p(0.5, 0.8);
    size_t start = 0;
    std::vector<size_t> tout = mesh.search(p, start);
    
    std::vector<MeshPoint> t_coords = mesh.coordsOfTriag(tout.back());
    std::cout << "Found in triangle " << tout.back() << std::endl;
    for (int i=0; i< t_coords.size(); i++){
        std::cout << t_coords[i].x_ << "," << t_coords[i].y_ << std::endl;
    }
    
    FILE * hist;
    hist = fopen("search.txt", "w");
    for(int i=0; i<tout.size(); i++){
        fprintf(hist, "%i\n", tout[i]);
    }
    fclose(hist);
}
