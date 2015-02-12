#include "mesh.hpp"
#include <fstream>
#include <iostream>
#include <limits>
#include <algorithm>
#include <assert.h>
template class vector<Facet>;
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
    auto it = facets.begin();
    for (auto cit = it + 1; cit != facets.end(); cit++) {
        if (*it != *cit) {
            it = cit;
        } else {
            it->getTets()->push_back(
                cit->getTets()->at(0));
        }
    }
    auto uit = unique(facets.begin(), facets.end());
    facets.erase(uit,facets.end());
    for (auto &f : facets)
        if (f.getTets()->size() == 1) {
            f.calcNormal();
            boundary_facets.push_back(f);
        }
}

Mesh::Mesh(string filename) {
    read_points(filename+".node");
    read_tets(filename+".ele");
    build_facets();
}

void Mesh::normalizeForDrawing() {
    double center[3] = {0, 0, 0};
    double c_max[3] = {-1e10,-1e10,-1e10};
    double c_min[3]  = {1e10, 1e10, 1e10};
    for (auto &p : points) {
        double *c = p.getC();
        for (int i=0; i<3; i++) {
            center[i]+=c[i];
            if (c[i] > c_max[i]) c_max[i] = c[i];
            if (c[i] < c_min[i]) c_min[i] = c[i];
        }
    }
    double width[3];
    for (int i=0; i<3; i++){
        center[i] /= points.size();
        width[i] = (c_max[i] - c_min[i])/2.0;
    }

    for (auto &p : points) {
        double *c = p.getC();
        for (int i=0; i<3; i++) {
            c[i] = (c[i] - center[i])/width[i];
        }
    }
}

void Facet::calcNormal() {
    double *p1 = points[0]->getC();
    double *p2 = points[1]->getC();
    double *p3 = points[2]->getC();

    double v1[3] = {p2[0]-p1[0],p2[1]-p1[1],p2[2]-p1[2]};
    double v2[3] = {p3[0]-p1[0],p3[1]-p1[1],p3[2]-p1[2]};
    normal[0] = v1[1]*v2[2]-v1[2]*v2[1];
    normal[1] = v1[2]*v2[0]-v1[0]*v2[2];
    normal[2] = v1[0]*v2[1]-v1[1]*v2[0];

    auto tet_points = tets[0]->getPoints();
    Point *extra = NULL;
    for (int i=0; i<4; i++) {
        if ((tet_points[i] != points[0]) &&
            (tet_points[i] != points[1]) &&
            (tet_points[i] != points[2])) {
            extra = tet_points[i];
            break;
        }
    }
    assert(extra);
    double* p_extra = extra->getC();
    double v_check[3] = {p_extra[0]-p1[0],p_extra[1]-p1[1],p_extra[2]-p1[2]};
    double v = 0;
    for (int i=0; i<3; i++)
        v += v_check[i]*normal[i];
    if (v<0) {
        for (int i=0; i<3; i++)
            normal[i] = -normal[i];
    }
}
