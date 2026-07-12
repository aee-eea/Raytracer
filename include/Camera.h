#pragma once
#include <glm/glm.hpp>
#include <memory>
#include <vector>
#include "IHit.h"
#include "VectorHelpers.h"
#include <glm/gtc/quaternion.hpp>
#include "Ray.h"

namespace rt{
class RaytracerCamera{
public:
    RaytracerCamera(int imageWidth, int imageHeight);

    void addPosition(glm::dvec3 newPos);
    void addRotation(double deltaYaw,double deltaPitch,double deltaRoll);
    void lookAt(glm::dvec3 newDir,glm::dvec3 up);
    void changeLens(double newDefocusAngle, double newFocusDist);
    void changeFov(double newFov);
    void changeImageSize(int newWidth, int newHeight);
    Ray generateRayForPixel(int x, int y);

    glm::dvec3 getUp() const;
    glm::dvec3 getRight() const;
    glm::dvec3 getForward() const;
    
private:

    double vfov{90};
    glm::dvec3 cameraPos{0,0,0};
    glm::dquat orientation{1,0,0,0};

    double defocusAngle{0.5};
    double focusDist{2};

    double viewportHeight;
    double viewportWidth;
    int imageWidth{0};
    int imageHeight{0};

    glm::dvec3 viewportU;
    glm::dvec3 viewportV;
    glm::dvec3 defocusDiskU;
    glm::dvec3 defocusDiskV;
    glm::dvec3 pixelDeltaU;
    glm::dvec3 pixelDeltaV;
    glm::dvec3 viewportUpperLeftPos;
    glm::dvec3 firstPixelPos;
    glm::dvec3 cameraForward,cameraRight,cameraUp;
    void updateCamera();
};
}