//
// Created by Gordan Milovac on 12/8/23.
//

#include <stdexcept>
#include "camera.h"
#include <glm/gtc/matrix_transform.hpp>

Camera::Camera(const glm::vec4 &pos, const glm::vec4 &look,const glm::vec4 &up, float aspectRatio, float& heightAngle, float& farPlane, float& nearPlane) {

    // View Matrix
    glm::vec4 tx(1.0f,0.0f ,0.0f,0.0f);
    glm::vec4 ty(0.0f ,1.0f,0.0f ,0.0f);
    glm::vec4 tz(0.0f,0.0f,1.0f ,0.0f);
    glm::vec4 th(-pos[0],-pos[1],-pos[2],1.0f);
    glm::mat4 translate(tx,ty,tz,th);
    glm::vec3 w = glm::normalize(-glm::vec3(look));
    glm::vec3 v = glm::normalize(glm::vec3(up)-glm::dot(glm::vec3(up),w)*w);
    glm::vec3 u = glm::cross(v ,w);
    glm::vec4 w4(w,0.0f);
    glm::vec4 v4(v,0.0f) ;
    glm::vec4 u4(u,0.0f) ;
    glm::vec4 h4(0.0f, 0.0f, 0.0f, 1.0f);
    glm::mat4 rotate = glm::transpose(glm::mat4(u4,v4,w4,h4));
    viewMatrix = rotate*translate;

    // Projection Matrix
    projectionMatrix = glm::perspective(heightAngle, aspectRatio, nearPlane, farPlane);
}

glm::mat4 Camera::getViewMatrix() const {
    return viewMatrix;
}

glm::mat4 Camera::getProjectionMatrix() const {
    return projectionMatrix;
}
