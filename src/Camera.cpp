#include "Camera.h"
#include "VectorHelpers.h"
#include "Raytracer.h"
#include "Ray.h"
#include <glm/glm.hpp>



RaytracerCamera::RaytracerCamera(int imageWidth, int imageHeight){
    changeImageSize(imageWidth,imageHeight);
    lookAt(glm::vec3{0,0,-1},glm::vec3{0,1,0});

    addPosition(glm::vec3{2.5,3,1.5});
}
void RaytracerCamera::addPosition(glm::vec3 newPos){
    cameraPos += newPos;
    updateCamera();
}
void RaytracerCamera::addRotation(float deltaYaw,float deltaPitch,float deltaRoll){
    glm::quat qYaw   = glm::angleAxis(glm::radians(deltaYaw),   glm::vec3(0,1,0));
    glm::quat qPitch = glm::angleAxis(glm::radians(deltaPitch), cameraRight);
    glm::quat qRoll  = glm::angleAxis(glm::radians(deltaRoll),  cameraForward);

    glm::quat delta = qYaw * qPitch * qRoll;

    orientation = glm::normalize(delta * orientation);
    updateCamera();
}
void RaytracerCamera::lookAt(glm::vec3 newDir, glm::vec3 up){
    orientation = glm::quatLookAt(glm::normalize(newDir - cameraPos), up);
    updateCamera();
}
void RaytracerCamera::updateCamera(){
    float theta = glm::radians(vfov);
    float h = glm::tan(theta/2);
    viewportHeight = 2 * h * focusDist;
    viewportWidth = viewportHeight * (static_cast<float>(imageWidth) / imageHeight);
    cameraForward = glm::normalize(orientation * glm::vec3(0,0,-1));
    cameraRight = glm::normalize(orientation * glm::vec3(1,0,0));
    cameraUp = glm::normalize(orientation * glm::vec3(0,1,0));

    viewportU = viewportWidth * cameraRight;
    viewportV = viewportHeight * -cameraUp;

    pixelDeltaU = viewportU / static_cast<float>(imageWidth);
    pixelDeltaV = viewportV / static_cast<float>(imageHeight);

    viewportUpperLeftPos = cameraPos
                            - (focusDist * -cameraForward)
                            - (viewportU / 2.0f)
                            - (viewportV / 2.0f);
    firstPixelPos = viewportUpperLeftPos + (pixelDeltaU * 0.5f) + (pixelDeltaV * 0.5f);

    float defocusRadius = focusDist * glm::tan(glm::radians(defocusAngle / 2));
    defocusDiskU = cameraRight * defocusRadius;
    defocusDiskV = cameraUp * defocusRadius;
}
void RaytracerCamera::changeLens(float newDefocusAngle, float newFocusDist){
    defocusAngle = newDefocusAngle;
    focusDist = newFocusDist;
    updateCamera();
}
void RaytracerCamera::changeFov(float newFov){
    vfov = newFov;
    updateCamera();
}
void RaytracerCamera::changeImageSize(int newWidth, int newHeight){
    imageWidth = newWidth;
    imageHeight = newHeight;
    updateCamera();
}
Ray RaytracerCamera::generateRayForPixel(int x, int y){
    glm::vec3 offset = sampleSquare();
    glm::vec3 pixelCenter = firstPixelPos + (pixelDeltaU * (static_cast<float>(x) + offset.x)) + (pixelDeltaV * (static_cast<float>(y) + offset.y));
    glm::vec3 origin = cameraPos;
    if(defocusAngle > 0){
        glm::vec3 random = randomInUnitDisk();
        origin = cameraPos + (random.x * defocusDiskU) + (random.y * defocusDiskV);
    }
    glm::vec3 rayDir = pixelCenter - origin;
    Ray ray (origin, glm::normalize(rayDir));
    return ray;
}

glm::vec3 RaytracerCamera::getUp() const{
    return cameraUp;
}
glm::vec3 RaytracerCamera::getRight() const{
    return cameraRight;
}
glm::vec3 RaytracerCamera::getForward() const{
    return cameraForward;
}