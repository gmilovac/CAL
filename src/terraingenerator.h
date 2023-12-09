#pragma once

#include <vector>
#include "rgba.h"
#include "glm/glm.hpp"

class TerrainGenerator
{
public:
    bool m_wireshade;

    TerrainGenerator();
    ~TerrainGenerator();
    int getResolution() { return m_resolution; };
    std::vector<float> generateTerrain(std::vector<glm::vec4> canvas, std::vector<float> noiseData, std::vector<float> heightData);

private:

    // Member variables for terrain generation. You will not need to use these directly.
    std::vector<glm::vec2> m_randVecLookup;
    int m_resolution;
    int m_lookupSize;
    std::vector<glm::vec4> m_canvas;
    std::vector<float> m_heightMap;
    float getHeightMap(int x, int y);

    uint8_t fToUint(float x);
    bool rgbEquals(glm::vec4 colVec4, RGBA rgba);

    // Samples the (infinite) random vector grid at (row, col)
    glm::vec2 sampleRandomVector(int row, int col);

    // Takes a grid coordinate (row, col), [0, m_resolution), which describes a vertex in a plane mesh
    // Returns a normalized position (x, y, z); x and y in range from [0, 1), and z is obtained from getHeight()
    glm::vec3 getPosition(int row, int col, float height, float noise);

    // ================== Students, please focus on the code below this point

    // Takes a normalized (x, y) position, in range [0,1)
    // Returns a height value, z, by sampling a noise function
    float getHeight(float x, float y, float height, float noise);

    // Computes the normal of a vertex by averaging neighbors
    glm::vec3 getNormal(int row, int col, float height, float noise);

    // Computes color of vertex using normal and, optionally, position
    glm::vec3 getColor(glm::vec3 normal, glm::vec3 position);

    // Computes the intensity of Perlin noise at some point
    float computePerlin(float x, float y);
};
