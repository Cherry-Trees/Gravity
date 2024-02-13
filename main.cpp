#include <SFML/Graphics.hpp>
#include <iostream>
#include <math.h>
#include <vector>
#include <string>
#include <algorithm>
#include "CGManager.h"

using namespace sf;

double fRand(double fMin, double fMax) {
    double f = (double)rand() / RAND_MAX;
    return fMin + f * (fMax - fMin);
}

int main()
{


    int LOD0_shape[2] = {1296, 1296};
    int LOD1_shape[2] = {216, 216};
    int LOD2_shape[2] = {36, 36};
    int LOD3_shape[2] = {6, 6};
    int xlim[2] = {0, 1296};
    int ylim[2] = {0, 1296};
    float delta = 1296 / 1296.0;

    CellGrid LOD0 = CellGrid(LOD0_shape, xlim, ylim);
    CellGrid LOD1 = CellGrid(LOD1_shape, xlim, ylim);
    CellGrid LOD2 = CellGrid(LOD2_shape, xlim, ylim);
    CellGrid LOD3 = CellGrid(LOD3_shape, xlim, ylim);

    CGManager CG = CGManager(LOD0, LOD1, LOD2, LOD3);
    Body b1 = Body(10000, 500, 500);
    Body b2 = Body(10000, 600, 200);
    CG.addBody(b1);
    CG.addBody(b2);

    RenderWindow window(VideoMode(1296, 1296), "SFML works!");

    while (window.isOpen())
    {   
        window.clear();
        Event event;
        while (window.pollEvent(event))
        {
            if (event.type == Event::Closed)
                window.close();
            
            if (event.type == Event::MouseMoved) {

                if (Mouse::isButtonPressed(Mouse::Left)) {
                    auto mousePos = Mouse::getPosition();
                    //bl.push_back(Body2(1e13, mousePos.x - 700, mousePos.y - 250));
                    Body body = Body(8000, mousePos.x - 700, mousePos.y - 250);
                    CG.addBody(body);
                }
            }
        }

        CG.sampleBodiesAcrossLODS();

        for (int i=0; i<CG.bodyArray.size(); i++) 
            window.draw(CG.bodyArray[i].sprite);

        window.display();
    }

    return 0;
}

// TO RUN:

// g++ -I"C:\SFML-2.6.1\include" -c main.cpp -o main.o
// g++ main.o -o app.exe -L"C:\SFML-2.6.1\lib" -lsfml-graphics -lsfml-window -lsfml-system -lsfml-main -mwindows

// OR

// MinGW32-make compile
// MinGW32-make link

// OR

// MinGW32-make all
