#include "terraingenerator.h"

#include <cmath>
#include <iostream>
#include "glm/glm.hpp"
#include "rgba.h"
#include "mainwindow.h"

// Constructor
TerrainGenerator::TerrainGenerator()
{
  m_wireshade = false; // STENCIL CODE
  cell = cell;

  // Define resolution of terrain generation
  m_resolution = 512;


    // Generate random vector lookup table
  m_lookupSize = 1024;
  m_randVecLookup.reserve(m_lookupSize);

  // Initialize random number generator
  std::srand(1230);

  // Populate random vector lookup table
  for (int i = 0; i < m_lookupSize; i++)
  {
    m_randVecLookup.push_back(glm::vec2(std::rand() * 2.0 / RAND_MAX - 1.0,
                                        std::rand() * 2.0 / RAND_MAX - 1.0));
    }
}

// Destructor
TerrainGenerator::~TerrainGenerator()
{
    m_randVecLookup.clear();
}

// Helper for generateTerrain()
void addPointToVector(glm::vec3 point, std::vector<float>& vector) {
    vector.push_back(point.x);
    vector.push_back(point.y);
    vector.push_back(point.z);
}

// Generates the geometry of the output triangle mesh
std::vector<float> TerrainGenerator::generateTerrain(std::vector<glm::vec4> canvas,  std::vector<float> noiseData, std::vector<float> heightData) {
    std::vector<float> verts;
    m_resolution = 512; //int(sqrt(flat.size()));
    verts.reserve(m_resolution * m_resolution * 6);
    m_heights.reserve(m_resolution * m_resolution);

    // Stores height positions for each x,y coordinate to increase efficiency during normal calculations
    for (int x=0; x<m_resolution; x++) {
        for (int y=0; y<m_resolution; y++) {
            int row = y;
            int col = (m_resolution-1) - x;
            storeHeightPositions(x,y, heightData[col+row*m_resolution], noiseData[col+row*m_resolution]);
        }
    }

    for(int x = 0; x < m_resolution; x++) {
        for(int y = 0; y < m_resolution; y++) {
            int x1 = x;
            int y1 = y;
            int x2 = x + 1;
            int y2 = y + 1;

            int row1 = y1;
            int col1 = (m_resolution-1) - x1; // flips horizontally - images coming out mirrored before
            int row2 = y2;
            int col2 = (m_resolution-1) - x2;
            row2 = std::clamp(row2,0,m_resolution-1);
            col2 = std::clamp(col2,0,m_resolution-1);

            glm::vec4 c1 = canvas[col1+row1*m_resolution];
            if (c1.x==1.f && c1.y==1.f && c1.z==1.f) {continue;}

            glm::vec3 p0 = getPosition(x1,y1);
            glm::vec3 p1 = getPosition(x2,y1);
            glm::vec3 p2 = getPosition(x2,y2);
            glm::vec3 p3 = getPosition(x1,y2);

            if (rgbEquals(c1,MainWindow::WATER_COLOR)) {
                float oceanThreshold = 0.01f;
                if (p1.z > oceanThreshold || p2.z > oceanThreshold || p3.z > oceanThreshold || p0.z > oceanThreshold) {
                    c1 = glm::vec4(0.988f,0.776f,0.376f,1.f); // sand color
                }
            }

            float mountainThreshold = 0.12;
            if (heightData[col1+row1*m_resolution] > mountainThreshold || heightData[col2+row1*m_resolution] > mountainThreshold ||
                heightData[col2+row2*m_resolution] > mountainThreshold || heightData[col1+row2*m_resolution] > mountainThreshold) {
                c1 = glm::vec4(.5,.5,.5,1); // mountain color
            }

            if (p1.z > 0.25f || p2.z > 0.25f || p3.z > 0.25f || p0.z > 0.25f) {
                c1 = glm::vec4(1.f,1.f,1.f,1.f); // snow color
            }

            glm::vec3 n0 = getNormal(x1,y1);
            glm::vec3 n1 = getNormal(x2,y1);
            glm::vec3 n2 = getNormal(x2,y2);
            glm::vec3 n3 = getNormal(x1,y2);

            addPointToVector(p0, verts);
            addPointToVector(n0, verts);
            addPointToVector(glm::vec3(c1), verts);

            addPointToVector(p1, verts);
            addPointToVector(n1, verts);
            addPointToVector(glm::vec3(c1), verts);

            addPointToVector(p2, verts);
            addPointToVector(n2, verts);
            addPointToVector(glm::vec3(c1), verts);

            // tris 2
            addPointToVector(p0, verts);
            addPointToVector(n0, verts);
            addPointToVector(glm::vec3(c1), verts);

            addPointToVector(p2, verts);
            addPointToVector(n2, verts);
            addPointToVector(glm::vec3(c1), verts);

            addPointToVector(p3, verts);
            addPointToVector(n3, verts);
            addPointToVector(glm::vec3(c1), verts);

        }
    }
    return verts;
}

// Samples the (infinite) random vector grid at (row, col)
glm::vec2 TerrainGenerator::sampleRandomVector(int row, int col) {
    std::hash<int> intHash;
    int index = intHash(row * 41 + col * 43) % m_lookupSize;
    return m_randVecLookup.at(index);
}

// Stores the height value (float) for each x,y coordinate
void TerrainGenerator::storeHeightPositions(int xIn, int yIn, float height, float noise) {
    float x = 1.f * xIn / m_resolution;
    float y = 1.f * yIn / m_resolution;
    m_heights[xIn+yIn*m_resolution] = abs(getHeight(x, y, height, noise));
}

// Gets positon x,y,h and returns as vec3
glm::vec3 TerrainGenerator::getPosition(int xIn, int yIn) {
    int x0 = std::clamp(xIn, 0, m_resolution-1);
    int y0 = std::clamp(yIn, 0, m_resolution-1);

    float x = 1.f * xIn / m_resolution;
    float y = 1.f * yIn / m_resolution;
    float h = m_heights[x0+y0*m_resolution];
    return glm::vec3(x,y,h);
}

bool TerrainGenerator::rgbEquals(glm::vec4 colVec4, RGBA rgba) {
    if (fToUint(colVec4[0]) == rgba.r){
        if (fToUint(colVec4[1]) == rgba.g){
            if (fToUint(colVec4[2]) == rgba.b){
                return true;
            }
        }
    }
    return false;
}

uint8_t TerrainGenerator::fToUint(float x) {
    int a = int(round(x*255.f));
    return std::min(std::max(a,0),255);
}

float ease(float alpha) {
    return 3.f*pow(alpha,2) - 2.f*pow(alpha,3);
}

// Helper for computePerlin() and, possibly, getColor()
float interpolate(float A, float B, float alpha) {
    return A + ease(alpha)*(B-A);
}

// Takes a normalized (x, y) position, in range [0,1)
// Returns a height value, z, by sampling a noise function
float TerrainGenerator::getHeight(float x, float y, float height, float noise) {
    x = 2*x;
    y = 2*y;
    float z1 = computePerlin(x, y);
    float z2 = computePerlin(x*2,y*2)/2;
    float z3 = computePerlin(x*4,y*4)/4;
    float z4 = computePerlin(x*8,y*8)/8;
    float z5 = computePerlin(x*16,y*16)/16;

    return height+noise*(z1+z2+z3+z4+z5);
}

// Computes the normal of a vertex by averaging neighbors
glm::vec3 TerrainGenerator::getNormal(int x, int y) {

    glm::vec3 mm = getPosition(x, y);
    glm::vec3 sumNormals(0.f,0.f,0.f); // center vertex

    glm::vec3 tl = getPosition(x - 1, y - 1); //top left
    glm::vec3 ml = getPosition(x + 0, y - 1); //mid left
    glm::vec3 bl = getPosition(x + 1, y - 1); // bottom left
    glm::vec3 bm = getPosition(x + 1, y - 0); // bottom mid
    glm::vec3 br = getPosition(x + 1, y + 1); // bottom right
    glm::vec3 mr = getPosition(x + 0, y + 1); // mid right
    glm::vec3 tr = getPosition(x - 1, y + 1); // top right
    glm::vec3 tm = getPosition(x - 1, y - 0); // top mid

    sumNormals = sumNormals + glm::cross(tl-mm,ml-mm);
    sumNormals = sumNormals + glm::cross(ml-mm,bl-mm);
    sumNormals = sumNormals + glm::cross(bl-mm,bm-mm);
    sumNormals = sumNormals + glm::cross(bm-mm,br-mm);
    sumNormals = sumNormals + glm::cross(br-mm,mr-mm);
    sumNormals = sumNormals + glm::cross(mr-mm,tr-mm);
    sumNormals = sumNormals + glm::cross(tr-mm,tm-mm);
    sumNormals = sumNormals + glm::cross(tm-mm,tl-mm);
    return glm::normalize(sumNormals);
}

struct Point {
    int x;
    int y;
};

struct Offset {
    float x;
    float y;
};

int floorInt(float x) {
    return static_cast<int>(floor(x));
}
// Computes the intensity of Perlin noise at some point
float TerrainGenerator::computePerlin(float x, float y) {
    Point p1 = {floorInt(x),floorInt(y)}; //top left
    Point p2 = {floorInt(x)+1,floorInt(y)}; //top right
    Point p3 = {floorInt(x)+1,floorInt(y)+1}; //bottom right
    Point p4 = {floorInt(x),floorInt(y)+1}; //bottom left

    glm::vec2 o1(x-p1.x,y-p1.y);
    glm::vec2 o2(x-p2.x,y-p2.y);
    glm::vec2 o3(x-p3.x,y-p3.y);
    glm::vec2 o4(x-p4.x,y-p4.y);

    float A = glm::dot(o1,sampleRandomVector(p1.x,p1.y)); // dot product between top-left direction and its offset
    float B = glm::dot(o2,sampleRandomVector(p2.x,p2.y)); // dot product between top-right direction and its offset
    float C = glm::dot(o3,sampleRandomVector(p3.x,p3.y)); // dot product between bottom-right direction and its offset
    float D = glm::dot(o4,sampleRandomVector(p4.x,p4.y)); // dot product between bottom-left direction and its offset

    float a = interpolate(interpolate(A, B, o1[0]), interpolate(D, C, o1[0]), o1[1]);
    return a;
}
