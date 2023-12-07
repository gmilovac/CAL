#include "terraingenerator.h"

#include <cmath>
#include <iostream>
#include "glm/glm.hpp"
#include "rgba.h"

// Constructor
TerrainGenerator::TerrainGenerator()
{
  // Task 8: turn off wireframe shading
  m_wireshade = false; // STENCIL CODE
  // m_wireshade = false; // TA SOLUTION

  // Define resolution of terrain generation
  m_resolution = 512;
  m_heightMap.reserve(m_resolution*m_resolution);

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
std::vector<float> TerrainGenerator::generateTerrain(std::vector<glm::vec4> canvas) {
    m_canvas = canvas;
    std::vector<float> verts;
    m_resolution = 512; //int(sqrt(flat.size()));
    verts.reserve(m_resolution * m_resolution * 6);

    for (int x = 0; x < m_resolution; x++) {
    for (int y = 0; y < m_resolution; y++) {
        int row = y;
        int col = (m_resolution-1) - x; // flips horizontally - images coming out mirrored before
        int pixel = col+row*m_resolution;
        m_heightMap[x+y*m_resolution] = canvas[pixel].y;
    }
    }

    for(int x = 0; x < m_resolution; x++) {
        for(int y = 0; y < m_resolution; y++) {

        int row = y;
        int col = (m_resolution-1) - x; // flips horizontally - images coming out mirrored before
            int pixel = col+row*m_resolution;

            glm::vec4 color = canvas[pixel];//glm::vec4(1.f,0.f,0.f,1.f);//flat[pixel];
            if (color.x==1.f && color.y==1.f && color.z==1.f) {
                continue;
            }

            int x1 = x;
            int y1 = y;

            int x2 = x + 1;
            int y2 = y + 1;



            glm::vec3 p1 = getPosition(x1,y1);
            glm::vec3 p2 = getPosition(x2,y1);
            glm::vec3 p3 = getPosition(x2,y2);
            glm::vec3 p4 = getPosition(x1,y2); // IN X,Z,Y format or (vec2 coordinate, height) format


            glm::vec3 n1 = getNormal(x1,y1);
            glm::vec3 n2 = getNormal(x2,y1);
            glm::vec3 n3 = getNormal(x2,y2);
            glm::vec3 n4 = getNormal(x1,y2);

            // tris 1
            // x1y1z1
            // x2y1z2
            // x2y2z3
            addPointToVector(p1, verts);
            addPointToVector(n1, verts);
            addPointToVector(glm::vec3(color), verts);
            //addPointToVector(getColor(n1, p1), verts);

            addPointToVector(p2, verts);
            addPointToVector(n2, verts);
            addPointToVector(glm::vec3(color), verts);
            //addPointToVector(getColor(n2, p2), verts);

            addPointToVector(p3, verts);
            addPointToVector(n3, verts);
            addPointToVector(glm::vec3(color), verts);
            //addPointToVector(getColor(n3, p3), verts);

            // tris 2
            // x1y1z1
            // x2y2z3
            // x1y2z4
            addPointToVector(p1, verts);
            addPointToVector(n1, verts);
            addPointToVector(glm::vec3(color), verts);
            //addPointToVector(getColor(n1, p1), verts);

            addPointToVector(p3, verts);
            addPointToVector(n3, verts);
            addPointToVector(glm::vec3(color), verts);
            //addPointToVector(getColor(n3, p3), verts);

            addPointToVector(p4, verts);
            addPointToVector(n4, verts);
            addPointToVector(glm::vec3(color), verts);
            //addPointToVector(getColor(n4, p4), verts);
        }
    }
    return verts;
}

// Samples the (infinite) random vector grid at (row, col)
glm::vec2 TerrainGenerator::sampleRandomVector(int row, int col)
{
    std::hash<int> intHash;
    int index = intHash(row * 41 + col * 43) % m_lookupSize;
    return m_randVecLookup.at(index);
}

// Takes a grid coordinate (row, col), [0, m_resolution), which describes a vertex in a plane mesh
// Returns a normalized position (x, y, z); x and y in range from [0, 1), and z is obtained from getHeight()
glm::vec3 TerrainGenerator::getPosition(int row, int col) {
    // Normalizing the planar coordinates to a unit square
    // makes scaling independent of sampling resolution.
    float x = 1.0 * row / m_resolution;
    float y = 1.0 * col / m_resolution;
    int y0 = m_resolution - 1 - row;
    int x0 = col; //(m_resolution-1) - col; // flips horizontally - images coming out mirrored before
    int pixel = x0+y0*m_resolution;
    pixel = std::clamp(pixel,0,int(m_canvas.size())-1);
    //std::cout << m_canvas[pixel].x;
    float z = 0; //getHeightMap(row,col);// m_canvas[pixel].x;//0.1f*m_canvas[pixel].x - 0.2f*m_canvas[pixel].z;//getHeight(x, y);
    return glm::vec3(x,y,z);
}

// ================== Students, please focus on the code below this point

float TerrainGenerator::getHeightMap(int x, int y) {
    return m_heightMap[x+y*m_resolution];
}

float ease(float alpha) {
    return 3.f*pow(alpha,2) - 2.f*pow(alpha,3);
}
// Helper for computePerlin() and, possibly, getColor()
float interpolate(float A, float B, float alpha) {
    // Task 4: implement your easing/interpolation function below

    return A + ease(alpha)*(B-A);
}

// Takes a normalized (x, y) position, in range [0,1)
// Returns a height value, z, by sampling a noise function
float TerrainGenerator::getHeight(float x, float y) {

    // Task 6: modify this call to produce noise of a different frequency
    float z1 = computePerlin(x, y);
    float z2 = computePerlin(x*2,y*2)/2;
    float z3 = computePerlin(x*4,y*4)/4;
    float z4 = computePerlin(x*8,y*8)/8;
    float z5 = computePerlin(x*16,y*16)/16;

    // Task 7: combine multiple different octaves of noise to produce fractal perlin noise

    // Return 0 as placeholder
    return z1+z2+z3+z4+z5;
}
//    Task 6:
//    Scaling z, the output of getHeight changes the amplitude of the noise (directly proportional)
//    Larger number means greater peaks/valleys, since z is the output height it scales the heightmap
//    Scaling x,y, the inputs of interpolate changes the frequency of the noise (directly proportional)
//    Scaling x adjusts the frequency in the x direction, increasing it traverses the noise function faster
//    making the frequency higher and likewise with y.

// Computes the normal of a vertex by averaging neighbors
glm::vec3 TerrainGenerator::getNormal(int row, int col) {
    // Task 9: Compute the average normal for the given input indices

    glm::vec3 V = getPosition(row,col);
    glm::vec3 sumNormals(0.f,0.f,0.f);

//    glm::vec3 n1 = getPosition(row - 1, col - 1);
//    glm::vec3 n2 = getPosition(row - 1, col + 0);
//    glm::vec3 n3 = getPosition(row - 1, col + 1);
//    glm::vec3 n4 = getPosition(row + 0, col + 1);
//    glm::vec3 n5 = getPosition(row + 1, col + 1); // clockwise rotation starting at row-1 col-1
//    glm::vec3 n6 = getPosition(row + 1, col + 0); // Why doesnt this work???!!!
//    glm::vec3 n7 = getPosition(row + 1, col - 1);
//    glm::vec3 n8 = getPosition(row + 0, col - 1);

    glm::vec3 n1 = getPosition(row - 1, col - 1);
    glm::vec3 n2 = getPosition(row + 0, col - 1);
    glm::vec3 n3 = getPosition(row + 1, col - 1);
    glm::vec3 n4 = getPosition(row + 1, col - 0);
    glm::vec3 n5 = getPosition(row + 1, col + 1);
    glm::vec3 n6 = getPosition(row + 0, col + 1);
    glm::vec3 n7 = getPosition(row - 1, col + 1);
    glm::vec3 n8 = getPosition(row - 1, col - 0);

    sumNormals = sumNormals + glm::cross(n1-V,n2-V);
    sumNormals = sumNormals + glm::cross(n2-V,n3-V);
    sumNormals = sumNormals + glm::cross(n3-V,n4-V);
    sumNormals = sumNormals + glm::cross(n4-V,n5-V);
    sumNormals = sumNormals + glm::cross(n5-V,n6-V);
    sumNormals = sumNormals + glm::cross(n6-V,n7-V);
    sumNormals = sumNormals + glm::cross(n7-V,n8-V);
    sumNormals = sumNormals + glm::cross(n8-V,n1-V);
    return glm::normalize(sumNormals);


//    glm::vec3 V = getPosition(row,col);
//    std::vector<glm::vec3> points(8);
//    int index = 0;
//    for (int i=-1;i<2;i++) {
//        for (int j=-1;j<2;j++) {
//            if (j!=0 && i!=0) {
//                points[index] = getPosition(row+i,col+j);
//                index++;
//            }
//        }
//    }

//    glm::vec3 sumNormals = (glm::cross((points[7]-V),(points[0]-V)));
//    for (int i=0;i<7;i++) {
//        sumNormals += glm::cross(points[i]-V,points[i+1]-V);
//    }
    //return glm::normalize(sumNormals);
}



// Computes color of vertex using normal and, optionally, position
glm::vec3 TerrainGenerator::getColor(glm::vec3 normal, glm::vec3 position) {
    // Task 10: compute color as a function of the normal and position
    float height = 0.1;
    float vertical = 0.5;
    if (position[2]>height) {
        if (glm::dot(normal,glm::vec3(0,0,1)) > vertical) {
            return glm::vec3(1,1,1);
        }
    }

    return glm::vec3(0.5,0.5,0.5);


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
    // Task 1: get grid indices (as ints)

    Point p1 = {floorInt(x),floorInt(y)}; //top left
    Point p2 = {floorInt(x)+1,floorInt(y)}; //top right
    Point p3 = {floorInt(x)+1,floorInt(y)+1}; //bottom right
    Point p4 = {floorInt(x),floorInt(y)+1}; //bottom left

    // Task 2: compute offset vectors
    glm::vec2 o1(x-p1.x,y-p1.y);
    glm::vec2 o2(x-p2.x,y-p2.y);
    glm::vec2 o3(x-p3.x,y-p3.y);
    glm::vec2 o4(x-p4.x,y-p4.y);

    // Task 3: compute the dot product between the grid point direction vectors and its offset vectors
    float A = glm::dot(o1,sampleRandomVector(p1.x,p1.y)); // dot product between top-left direction and its offset
    float B = glm::dot(o2,sampleRandomVector(p2.x,p2.y)); // dot product between top-right direction and its offset
    float C = glm::dot(o3,sampleRandomVector(p3.x,p3.y)); // dot product between bottom-right direction and its offset
    float D = glm::dot(o4,sampleRandomVector(p4.x,p4.y)); // dot product between bottom-left direction and its offset

    // Task 5: Debug this line to properly use your interpolation function to produce the correct value
    return interpolate(interpolate(A, B, o1[0]), interpolate(D, C, o1[0]), o1[1]);

    // Return 0 as a placeholder
    return 0;
}
