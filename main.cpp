#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include "mesh.hpp"

using namespace std;

int main(int argc, char **argv)
{
    string mesh_file(argv[1]);
    Mesh mesh(mesh_file);
    return 0;
}
