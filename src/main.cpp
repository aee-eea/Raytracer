#include <iostream>
#include "RaylibFrontend.h"
#include "Raytracer.h"

int main(){
    RaylibFrontend frontend(640,360,"Test");
    Raytracer raytracer(frontend);
    frontend.start(raytracer);
}