#include <iostream>
#include "RaylibFrontend.h"
#include "Raytracer.h"

int main(){
    RaylibFrontend frontend(800,400,"Test");
    Raytracer raytracer(frontend,2.0);
    frontend.start(raytracer);
}