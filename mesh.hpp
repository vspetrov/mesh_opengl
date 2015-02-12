#ifndef __MESH_HPP__
#define __MESH_HPP__
#include <vector>
#include <string>
#include <tuple>
#include <unordered_map>
#include <sstream>
using namespace std;

class Tet;
class Facet;

class Point{
    double c[3];
    int id;
    vector<Tet *> tets;
public:
    Point(){}
    Point(int _id, double x, double y, double z) {
        id = _id;
        c[0] = x; c[1] = y; c[2] = z;
    }
    void addTet(Tet *tet) { tets.push_back(tet); }
    vector<Tet *>& getTets() { return tets; }
    int getId()  { return id; }
    double *getC() { return c; }
};

class Tet{
    Point* points[4];
    int id;
public:
    Tet(){}
    Tet(int _id, Point *p1, Point *p2, Point *p3, Point *p4 ) {
        points[0] = p1;
        points[1] = p2;
        points[2] = p3;
        points[3] = p4;
        id = _id;
    }
    Point** getPoints() { return points; }
};


class Facet{
    int id[3];
    Point* points[3];
    vector<Tet *> tets;
    double normal[3];
public:
    Facet(){}
    Facet(Point* v1, Point* v2, Point* v3, Tet *tet) {
        points[0] = v1;
        points[1] = v2;
        points[2] = v3;
        tets.push_back(tet);
        id[0] = v1->getId();
        id[1] = v2->getId();
        id[2] = v3->getId();
    }
    Facet(Point* v1, Point* v2, Point* v3, vector<Tet *> tets) {
        points[0] = v1;
        points[1] = v2;
        points[2] = v3;
        tets = tets;
        id[0] = v1->getId();
        id[1] = v2->getId();
        id[2] = v3->getId();
    }
    double *getNormal() { return normal; }
    const int* getId() const { return id; }
    bool operator==(Facet &f) {
        const int *fid = f.getId();
        return (id[0] == fid[0]) &&
            (id[1] == fid[1]) && (id[2] == fid[2]);
    }
    bool operator!=(Facet &f) {
        return !(operator==(f));
    }
    bool operator< (const Facet &f) const {
        const int *fid = f.getId();
        if (id[0] != fid[0]) return id[0] < fid[0];
        else if (id[1] != fid[1]) return id[1] < fid[1];
        else return id[2] < fid[2];
    }
    string getIdStr() {
        stringstream iss;
        iss << id[0] << ":" << id[1] << ":" << id[2];
        return iss.str();
    }
    Point** getPoints()  { return points; }

    vector<Tet *>* getTets() { return &tets; }
    void calcNormal();
    // Facet& operator=(const Facet& f) {
    // }
};

class Mesh {
    vector<Point> points;
    vector<Tet>   tets  ;
    vector<Facet> facets;
    vector<Facet> boundary_facets;
    void read_points(string filename);
    void read_tets(string filename);
    void build_facets();
public:
    Mesh(string filename);
    vector<Point>& getPoints() { return points; }
    vector<Facet>& getBoundaryFacets() { return boundary_facets; }
    void normalizeForDrawing();
};
#endif
