# delta
Triangles, triangles.....

A meshing and searching program in C++. 

The (very fast) triangulation is built with the delaunator-cpp:
https://github.com/delfrrr/delaunator-cpp.
For any set of scatter coordinates in 2D, a mesh objects triangulates the 
domain, and then "walks" on the triangulation:

  - You want to locate point P in the triangulation.
  - Start from any triangle t, find point P0 that's in t. (centroid, for example)
  - Connect P and P0. Find the edge e in t that P-P0 intersects with
  - Walk to the other triangle that shares e with t
  - Repeat.

Since the consecutive calls to the "search" is likely to be correlated, it is 
recommended to use the result of previous search as the initial guess for the 
next. The complexity of the search is then amortized O(1), worst case O(N), 
where N is the number of scattered coordinates.

Interpolations are done with barycentric linear interpolations in triangles.

Example:
```
    typedef std::vector<double> VecDoub;
    // coordinates in format required by delaunator {x1, y1, x2, y2, ...}
    VecDoub coords = {-1, 1, 1, 1, 1, -1, -1, -1, 0, 0, -1, 0};
    // function values (to be interpolated) defined on vertices
    VecDoub val = {0, 1, 2, 3}

    Mesh mesh(coords, val);
    // example point to search for
    MeshPoint p(0.5, 0.8);
    // initial guess for the index of triangle
    size_t start = 0;
    std::vector<size_t> tout = mesh.search(p, start);
    
    // The last item in the search history is the final result
    std::vector<MeshPoint> t_coords = mesh.coordsOfTriag(tout.back());
    std::cout << "Found in triangle " << tout.back() << std::endl;
    for (int i=0; i< t_coords.size(); i++){
        std::cout << t_coords[i].x_ << "," << t_coords[i].y_ << std::endl;
    }
```

The following copyright notice has been asked to included by the authors of 
delaunator-cpp:
MIT License

Copyright (c) 2018 Volodymyr Bilonenko

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.
