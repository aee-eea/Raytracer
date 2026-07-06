#include "Camera.h"
#include "VectorHelpers.h"
#include "Raytracer.h"
#include "Ray.h"
#include <glm/glm.hpp>



RaytracerCamera::RaytracerCamera(int imageWidth, int imageHeight){
    changeImageSize(imageWidth,imageHeight);
    lookAt(glm::dvec3{0,0,-1},glm::dvec3{0,1,0});

    addPosition(glm::dvec3{2.5,3,1.5});
}
void RaytracerCamera::addPosition(glm::dvec3 newPos){
    cameraPos += newPos;
    updateCamera();
}
void RaytracerCamera::addRotation(double deltaYaw,double deltaPitch,double deltaRoll){
    glm::dquat qYaw   = glm::angleAxis(glm::radians(deltaYaw),   glm::dvec3(0,1,0));
    glm::dquat qPitch = glm::angleAxis(glm::radians(deltaPitch), cameraRight);
    glm::dquat qRoll  = glm::angleAxis(glm::radians(deltaRoll),  cameraForward);

    glm::dquat delta = qYaw * qPitch * qRoll;

    orientation = glm::normalize(delta * orientation);
    updateCamera();
}
void RaytracerCamera::lookAt(glm::dvec3 newDir, glm::dvec3 up){
    orientation = glm::quatLookAt(glm::normalize(newDir - cameraPos), up);
    updateCamera();
}
void RaytracerCamera::updateCamera(){
    double theta = glm::radians(vfov);
    double h = glm::tan(theta/2);
    viewportHeight = 2 * h * focusDist;
    viewportWidth = viewportHeight * (static_cast<double>(imageWidth) / imageHeight);
    cameraForward = glm::normalize(orientation * glm::dvec3(0,0,-1));
    cameraRight = glm::normalize(orientation * glm::dvec3(1,0,0));
    cameraUp = glm::normalize(orientation * glm::dvec3(0,1,0));

    viewportU = viewportWidth * cameraRight;
    viewportV = viewportHeight * -cameraUp;

    pixelDeltaU = viewportU / static_cast<double>(imageWidth);
    pixelDeltaV = viewportV / static_cast<double>(imageHeight);

    viewportUpperLeftPos = cameraPos
                            - (focusDist * -cameraForward)
                            - (viewportU / 2.0)
                            - (viewportV / 2.0);
    firstPixelPos = viewportUpperLeftPos + (pixelDeltaU * 0.5) + (pixelDeltaV * 0.5);

    double defocusRadius = focusDist * glm::tan(glm::radians(defocusAngle / 2));
    defocusDiskU = cameraRight * defocusRadius;
    defocusDiskV = cameraUp * defocusRadius;
}
void RaytracerCamera::changeLens(double newDefocusAngle, double newFocusDist){
    defocusAngle = newDefocusAngle;
    focusDist = newFocusDist;
    updateCamera();
}
void RaytracerCamera::changeFov(double newFov){
    vfov = newFov;
    updateCamera();
}
void RaytracerCamera::changeImageSize(int newWidth, int newHeight){
    imageWidth = newWidth;
    imageHeight = newHeight;
    updateCamera();
}
Ray RaytracerCamera::generateRayForPixel(int x, int y){
    glm::dvec3 offset = sampleSquare();
    glm::dvec3 pixelCenter = firstPixelPos + (pixelDeltaU * (static_cast<double>(x) + offset.x)) + (pixelDeltaV * (static_cast<double>(y) + offset.y));
    glm::dvec3 origin = cameraPos;
    if(defocusAngle > 0){
        glm::dvec3 random = randomInUnitDisk();
        origin = cameraPos + (random.x * defocusDiskU) + (random.y * defocusDiskV);
    }
    glm::dvec3 rayDir = pixelCenter - origin;
    Ray ray (origin, glm::normalize(rayDir));
    return ray;
}

glm::dvec3 RaytracerCamera::getUp() const{
    return cameraUp;
}
glm::dvec3 RaytracerCamera::getRight() const{
    return cameraRight;
}
glm::dvec3 RaytracerCamera::getForward() const{
    return cameraForward;
}