#pragma once

#include <vector>
#include "rgba.h"
#include "glm/glm.hpp"

class TerrainGenerator
{
public:
    bool m_wireshade;
    bool cell;

    TerrainGenerator();
    ~TerrainGenerator();
    int getResolution() { return m_resolution; };
    std::vector<float> generateTerrain(std::vector<glm::vec4> canvas, std::vector<float> noiseData, std::vector<float> heightData);

private:

    // Member variables for terrain generation. You will not need to use these directly.
    std::vector<glm::vec2> m_randVecLookup;
    int m_resolution;
    int m_lookupSize;
    std::vector<float> m_heights;
    uint8_t fToUint(float x);
    bool rgbEquals(glm::vec4 colVec4, RGBA rgba);

    // Samples the (infinite) random vector grid at (row, col)
    glm::vec2 sampleRandomVector(int row, int col);

    // Takes a grid coordinate (row, col), [0, m_resolution), which describes a vertex in a plane mesh
    // Returns a normalized position (x, y, z); x and y in range from [0, 1), and z is obtained from getHeight()
    glm::vec3 getPosition(int xIn, int yIn);
    void storeHeightPositions(int xIn, int yIn, float height, float noise);

    float getHeight(float x, float y, float height, float noise);

    // Computes the normal of a vertex by averaging neighbors
    glm::vec3 getNormal(int y, int x);

    // Computes the intensity of Perlin noise at some point
    float computePerlin(float x, float y);
};
