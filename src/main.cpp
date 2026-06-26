#include <iostream>
#include "RaylibFrontend.h"
#include "Raytracer.h"

int main(){
    RaylibFrontend frontend(1280,720,427,240,"Test");
    Raytracer raytracer(frontend);
    frontend.start(raytracer);
}