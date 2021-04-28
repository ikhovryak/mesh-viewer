
#include "mesh.h"
#include "stdio.h"
#include "string.h"
#include "stdlib.h"
#include <cmath>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>

using namespace std;
using namespace glm;
using namespace agl;

Mesh::Mesh() 
{
}

Mesh::~Mesh()
{
}

bool Mesh::loadPLY(const std::string& filename)
{
    string ply;
    string skipped_strs;
    
    ifstream file(filename);
    if (!file) // true if the file is valid
    {
        cout << "Cannot load file: " << filename << endl;
        return false;
    }

    file >> ply;

    if (ply != "ply") return false;

    // skipping formatting info "comment created by blender"
    while (skipped_strs != "vertex")
    {
        file >> skipped_strs;
    }
    file >> numVert;
    cout << numVert;

    while (skipped_strs != "face")
    {
        file >> skipped_strs;
    }
    file >> numPol;
    cout << numPol;
    

    while(skipped_strs != "end_header") {
        file >> skipped_strs;
    }

    
    float* all_vertices = new float[numVert * 3];
    float* all_normals = new float[numVert * 3];
    float minX = minY = minZ = maxX = maxY = maxZ = INFINITY;
    
    for (int i = 0; i < numVert*3; i+=3) {
        float x;
        float y;
        float z;
        float nx;
        float ny;
        float nz;
        file >> x >> y >> z >> nx >> ny >> nz;
        if (x < minX) minX = x;
        else if (x > maxX) maxX = x;
        if (y < minY) minY = y;
        else if (y > maxY) maxY = y;
        if (z < minZ) minZ = z;
        else if (z > maxZ) maxZ = z;
        all_vertices[i] = x;
        all_vertices[i+1] = y;
        all_vertices[i+2] = z;
        all_normals[i] = nx;
        all_normals[i + 1] = ny;
        all_normals[i + 2] = nz;
    }


    total_positions = new float[numPol * 3 * 3];
    total_normals = new float[numPol * 3 * 3];
    total_indices = new unsigned int[numPol * 3 * 3];

    for (int i = 0; i < numPol * 3 * 3; i++) {
        total_indices[i] = i;
    }
    cout << endl;
    for (int i = 0; i < numPol; i++) {
        int verId;
        file >> skipped_strs;
        file >> verId;
        
        if (i < 10) { 
            cout << "verID = " << verId << " = " << all_vertices[verId * 3 + 0] << " " << all_vertices[verId * 3 + 1] << " " << all_vertices[verId * 3 + 2] << endl; 
            cout << "posit = " << verId << " = " << total_positions[i * 3] << " " << total_positions[i * 3 + 1] << " " << total_positions[i * 3 + 2] << endl;

        }
        total_positions[i*3] = all_vertices[verId * 3];
        total_positions[i * 3 + 1] = all_vertices[verId * 3 + 1];
        total_positions[i * 3 + 2] = all_vertices[verId * 3 + 2];
        total_normals[i * 3] = all_normals[verId * 3];
        total_normals[i * 3 + 1] = all_normals[verId * 3 + 1];
        total_normals[i * 3 + 2] = all_normals[verId * 3 + 2];


        file >> verId;
        if (i < 10) { 
            cout << "verID = " << verId << " = " << all_vertices[verId * 3 + 0] << " " << all_vertices[verId * 3 + 1] << " " << all_vertices[verId * 3 + 2] << endl; 
            cout << "posit = " << verId << " = " << total_positions[i * 3 + 3] << " " << total_positions[i * 3 + 4] << " " << total_positions[i * 3 + 5] << endl;

        }

        total_positions[i * 3 + 3] = all_vertices[verId * 3];
        total_positions[i * 3 + 4] = all_vertices[verId * 3 + 1];
        total_positions[i * 3 + 5] = all_vertices[verId * 3 + 2];
        total_normals[i * 3 + 3] = all_normals[verId * 3];
        total_normals[i * 3 + 4] = all_normals[verId * 3 + 1];
        total_normals[i * 3 + 5] = all_normals[verId * 3 + 2];

        file >> verId;
        if (i < 10) { 
            cout << "verID = " << verId << " = " << all_vertices[verId * 3 + 0] << " " << all_vertices[verId * 3 + 1] << " " << all_vertices[verId * 3 + 2] << endl; 
            cout << "posit = " << verId << " = " << total_positions[i * 3 + 6] << " " << total_positions[i * 3 + 7] << " " << total_positions[i * 3 + 8] << endl;

        }

        total_positions[i * 3 + 6] = all_vertices[verId * 3];
        total_positions[i * 3 + 7] = all_vertices[verId * 3 + 1];
        total_positions[i * 3 + 8] = all_vertices[verId * 3 + 2];
        total_normals[i * 3 + 6] = all_normals[verId * 3];
        total_normals[i * 3 + 7] = all_normals[verId * 3 + 1];
        total_normals[i * 3 + 8] = all_normals[verId * 3 + 2];
    }
    cout << endl;
    for (int i = 0; i < 10; i++) {
        cout << total_positions[i*3] << " "<< total_positions[i*3+1] << " " << total_positions[i*3+2] <<endl;
    }
    return true;
}

glm::vec3 Mesh::getMinBounds() const
{
    return vec3(minX, minY, minZ);
}

glm::vec3 Mesh::getMaxBounds() const
{
    return vec3(maxX, maxY, maxZ);
}

int Mesh::numVertices() const
{
    return numVert;
}

int Mesh::numTriangles() const
{
    return numPol;
}

float* Mesh::positions() const
{
    return total_positions;
}

float* Mesh::normals() const
{
    return total_normals;
}

unsigned int* Mesh::indices() const
{
    return total_indices;
}