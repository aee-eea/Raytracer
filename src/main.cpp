#include <iostream>
#include "RaylibFrontend.h"
#include "Raytracer.h"

int main(){
    rt::Raytracer raytracer(960,540,50,50,10,128);
    RaylibFrontend frontend(raytracer,1920,1080,"Test");
    frontend.start();
}