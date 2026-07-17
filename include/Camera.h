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

    void addPosition(glm::vec3 newPos);
    void addRotation(float deltaYaw,float deltaPitch,float deltaRoll);
    void lookAt(glm::vec3 newDir,glm::vec3 up);
    void changeLens(float newDefocusAngle, float newFocusDist);
    void changeFov(float newFov);
    void changeImageSize(int newWidth, int newHeight);
    Ray generateRayForPixel(int x, int y);

    glm::vec3 getUp() const;
    glm::vec3 getRight() const;
    glm::vec3 getForward() const;
    
private:

    float vfov{90};
    glm::vec3 cameraPos{0,0,0};
    glm::quat orientation{1,0,0,0};

    float defocusAngle{0.5};
    float focusDist{2};

    float viewportHeight;
    float viewportWidth;
    int imageWidth{0};
    int imageHeight{0};

    glm::vec3 viewportU;
    glm::vec3 viewportV;
    glm::vec3 defocusDiskU;
    glm::vec3 defocusDiskV;
    glm::vec3 pixelDeltaU;
    glm::vec3 pixelDeltaV;
    glm::vec3 viewportUpperLeftPos;
    glm::vec3 firstPixelPos;
    glm::vec3 cameraForward,cameraRight,cameraUp;
    void updateCamera();
};
}