#include <iostream>
#include "RaylibFrontend.h"
#include "Raytracer.h"

int main(){
    Raytracer raytracer(480,270,10,50,8);
    RaylibFrontend frontend(raytracer,1920,1080,"Test");
    frontend.start();
}