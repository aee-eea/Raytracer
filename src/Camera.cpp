#include "Camera.h"
#include "Ray.h"
#include "VectorHelpers.h"
#include "Raytracer.h"
#include <glm/glm.hpp>


static glm::dvec3 sampleSquare();

RaytracerCamera::RaytracerCamera(int imageWidth, int imageHeight){
    changeImageSize(imageWidth,imageHeight);
    lookAt(glm::dvec3{0,0,-1},glm::dvec3{0,1,0});
    changePosition(glm::dvec3{0,0.5,0.5});
    changeDirection(0,-30,0);
}

glm::dvec3 RaytracerCamera::renderPixel(int x, int y, const Raytracer& env) const{
    glm::dvec3 finalColor = {0,0,0};
    
    for(int i = 0; i < samplesPerPixel; i++){
        glm::dvec3 offset = sampleSquare();
        glm::dvec3 pixelCenter = firstPixelPos + (pixelDeltaU * (static_cast<double>(x) + offset.x)) + (pixelDeltaV * (static_cast<double>(y) + offset.y));
        glm::dvec3 origin = cameraPos;
        if(defocusAngle > 0){
            glm::dvec3 random = randomInUnitDisk();
            origin = cameraPos + (random.x * defocusDiskU) + (random.y * defocusDiskV);
        }
        glm::dvec3 rayDir = pixelCenter - origin;

        Ray ray (origin, glm::normalize(rayDir));


        finalColor += ray.rayColor(env,Interval{0.001, infinity},maxDepth);
    }

    finalColor *= (1.0 / samplesPerPixel);
    return finalColor;
}

void RaytracerCamera::changeQuality(int newSamples, int newDepth){
    samplesPerPixel = newSamples;
    maxDepth = newDepth;
}
void RaytracerCamera::changePosition(glm::dvec3 newPos){
    cameraPos = newPos;
}
void RaytracerCamera::changeDirection(double deltaYaw,double deltaPitch,double deltaRoll){
    glm::dquat qYaw   = glm::angleAxis(glm::radians(deltaYaw),   glm::dvec3(0,1,0));
    glm::dquat qPitch = glm::angleAxis(glm::radians(deltaPitch), glm::dvec3(1,0,0));
    glm::dquat qRoll  = glm::angleAxis(glm::radians(deltaRoll),  glm::dvec3(0,0,1));

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
    cameraForward = orientation * glm::dvec3(0,0,-1);
    cameraRight = orientation * glm::dvec3(1,0,0);
    cameraUp = orientation * glm::dvec3(0,1,0);

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
void RaytracerCamera::changeLens(double newDefocusAngle, double newFocusDist){}
void RaytracerCamera::changeFov(double newFov){}
void RaytracerCamera::changeImageSize(int newWidth, int newHeight){
    imageWidth = newWidth;
    imageHeight = newHeight;
    updateCamera();
}

static glm::dvec3 sampleSquare(){
    return glm::dvec3(randomDouble() - 0.5, randomDouble() - 0.5, 0);
}