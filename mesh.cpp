#include "mesh.hpp"
#include <fstream>
#include <iostream>
#include <limits>
#include <algorithm>
#include <assert.h>
void Mesh::read_points(string filename) {

    int points_number;
    ifstream ifs;
    ifs.exceptions ( ifstream::failbit | ifstream::badbit );
    try{
        ifs.open(filename.c_str(), ios::in | ios::binary);
    }
    catch(ifstream::failure e){
        cout << "Error: Can not open input mesh file " << filename << ": " << e.what() << endl;
    }
    ifs >> points_number;
    points.resize(points_number);
    for (int i=0; i<points_number; i++) {
        ifs.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        int id;
        double c[3];
        ifs >> id >> c[0] >> c[1] >> c[2];
        points[id] = Point(id, c[0], c[1], c[2]);
    }
    ifs.close();

}

void Mesh::read_tets(string filename) {
    int tets_number;
    ifstream ifs;
    ifs.exceptions ( ifstream::failbit | ifstream::badbit );
    try{
        ifs.open(filename.c_str(), ios::in | ios::binary);
    }
    catch(ifstream::failure e){
        cout << "Error: Can not open input mesh file " << filename << ": " << e.what() << endl;
    }
    ifs >> tets_number;
    tets.resize(tets_number);
    for (int i=0; i<tets_number; i++) {
        ifs.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        int id;
        int v[4];
        ifs >> id >> v[0] >> v[1] >> v[2] >> v[3];
        tets[id] = Tet(id,
                       &points[v[0]],
                       &points[v[1]],
                       &points[v[2]],
                       &points[v[3]]);
    }
    ifs.close();
}


void Mesh::build_facets() {
    int combinations[4][3] = {{0,1,2},{0,1,3}, {0,2,3}, {1,2,3}};
    for (auto &t : tets) {
        Point** points = t.getPoints();
        for (int i=0; i<4; i++) {
            int *c = combinations[i];
            facets.push_back(Facet(points[c[0]],
                                   points[c[1]],
                                   points[c[2]],
                                   &t));
        }
    }
    sort(facets.begin(), facets.end());
    auto uit = unique(facets.begin(), facets.end());
    int non_uniq_size = facets.end() - uit;

    auto it = facets.begin();
    for (;uit != facets.end(); uit++) {
        if (*uit != *it) it++;
        assert(*uit == *it);
    }
}

Mesh::Mesh(string filename) {
    read_points(filename+".node");
    read_tets(filename+".ele");
    build_facets();
}
