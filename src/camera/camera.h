//
// Created by Gordan Milovac on 12/8/23.
//

#pragma once

#include <glm/glm.hpp>

class Camera {
public:
    Camera(const glm::vec4 &pos, const glm::vec4 &look,const glm::vec4 &up, float aspectRatio, float& heightAngle, float& farPlane, float& nearPlane);

    glm::mat4 getViewMatrix() const;

    glm::mat4 getProjectionMatrix() const;

private:
    glm::mat4 viewMatrix;
    glm::mat4 projectionMatrix;
};
