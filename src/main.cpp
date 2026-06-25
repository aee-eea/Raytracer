#include <iostream>
#include "RaylibFrontend.h"
#include "Raytracer.h"

int main(){
    RaylibFrontend frontend(1280,720,"Test");
    Raytracer raytracer(frontend,2.0);
    frontend.start(raytracer);
}