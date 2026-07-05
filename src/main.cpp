#include <iostream>
#include "RaylibFrontend.h"
#include "Raytracer.h"

int main(){
    Raytracer raytracer(960,540,10);
    RaylibFrontend frontend(raytracer,1920,1080,"Test");
    frontend.start();
}