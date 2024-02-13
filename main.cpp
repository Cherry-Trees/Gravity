#include <SFML/Graphics.hpp>
#include <iostream>
#include <math.h>
#include <vector>
#include <string>
#include <algorithm>
#include "CGManager.h"

using namespace sf;

// double norm(Vector2<double> A) {
//     return sqrt( ((A.x) * (A.x)) + ((A.y) * (A.y)) );
// }


// class Body2 {

//     public:

//         double m;
//         Vector2<double> r;
//         Vector2<double> v;
//         Vector2<double> a;
//         CircleShape sprite;
//         float size = 5;

//         Body2() {}

//         Body2(double m, double x0, double y0, double vx0, double vy0) : r{x0, y0}, v{vx0, vy0} {
//             this->m = m;
//             this->sprite = CircleShape(size);
//             this->sprite.setFillColor(Color::White);
//         }

//         Body2(double m, double x0, double y0) : r{x0, y0}, v{0, 0} {
//             this->m = m;
//             this->sprite = CircleShape(size);
//             this->sprite.setFillColor(Color::White);
//         }

//         void updateSprite() {
//             this->sprite.setPosition(this->r.x, this->r.y);
//             this->sprite.setFillColor(Color(std::min(norm(this->v)*25, 255.0), 0, std::max(255-norm(this->v)*25, 0.0)));

//         }
    
//     private:
        
// };



// // Calculate the norm of two Vectors
// double norm(Vector2<double> A, Vector2<double> B) {
//     return sqrt( ((B.x - A.x) * (B.x - A.x)) + ((B.y - A.y) * (B.y - A.y)) );
// }

// double rij(Body2 i, Body2 j) {
//     return norm(i.r, j.r);
// }

// Vector2<double> Fgij(Body2 i, Body2 j) {
//     return (j.r - i.r)*G*j.m*i.m / (rij(j, i)*rij(j, i)*rij(j, i));
// }

// Vector2<double> calculateAcceleration(Body2 i, Body2 j) {
//     return Fgij(i, j) / i.m;
// }

// void verlet(Body2 bl[], const int N, const double delta) {
//     for (int i=0; i<N; i++) {
//         for (int j=0; j<N; j++) {

//             if (i != j) {
//                 auto aij = calculateAcceleration(bl[i], bl[j]);
//                 bl[i].v += aij * delta;
//                 bl[i].r += bl[i].v * delta;
//                 bl[i].updateSprite();
//             }
//         }
//     }
// }

// double rij(const Body3 i, const Body3 j) {return norm(i.r, j.r);}
// Vector3 Fgij(const Body3 i, const Body3 j) {return (j.r - i.r)*G*j.m*i.m / (rij(j, i)*rij(j, i)*rij(j, i));}

// void verlet(Body3 &i, Body3 &j, Body3 &k, const double delta) {
//     auto fgij = Fgij(i, j);
//     auto fgik = Fgij(i, k);
//     auto fgjk = Fgij(j, k);

//     auto aij = fgij / i.m;
//     auto aji = fgij / -j.m;
//     auto aik = fgik / i.m;
//     auto aki = fgik / -k.m;
//     auto ajk = fgjk / j.m;
//     auto akj = fgjk / -k.m;

//     i.v += (aij + aik) * delta;
//     j.v += (aji + ajk) * delta;
//     k.v += (aki + akj) * delta;
//     i.r += i.v * delta;
//     j.r += j.v * delta;
//     k.r += k.v * delta;
// }

// void verlet(Body3 &i, Body3 &j, Body3 ast[], const int N_AST, const double delta) {
//     auto fgij = Fgij(i, j);
//     auto aitemp = i.a;
//     auto ajtemp = j.a;

//     i.a = fgij / i.m;
//     j.a = fgij / -j.m;
//     i.v += (i.a + aitemp) * delta * 0.5;
//     j.v += (j.a + ajtemp) * delta * 0.5;
//     i.r += i.v * delta + i.a * delta * delta * 0.5;
//     j.r += j.v * delta + j.a * delta * delta * 0.5;

//     for (int n=0; n<N_AST; n++) {
//         auto fgasti = Fgij(ast[n], i);
//         auto fgastj = Fgij(ast[n], j);
//         auto atemp = ast[n].a;

        
//         ast[n].a = (fgasti + fgastj) / ast[n].m;
//         ast[n].v += (atemp + ast[n].a) * delta * 0.5;
//         ast[n].r += ast[n].v * delta + ast[n].a * delta * delta * 0.5;
//     }
// }

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

        // for (Body &body : CG.bodyArray)
        //     window.draw(body.sprite);

        for (int i=0; i<CG.bodyArray.size(); i++) 
            window.draw(CG.bodyArray[i].sprite);
        
    


        
        // for (int i=0; i<bl.size(); i++) {
        //     for (int j=0; j<bl.size(); j++) {
        //         if (i != j && rij(bl[i], bl[j]) > 1) {
        //             auto aij = calculateAcceleration(bl[i], bl[j]);
        //             bl[i].v += aij * delta;
        //             bl[i].r += bl[i].v * delta;
        //             bl[i].updateSprite();
        //             window.draw(bl[i].sprite);
        //         }
        //     }
        // }

        

        // window.clear();
        // window.draw(shape);
        window.display();
    }

    return 0;
}

// #include <SFML/Window.hpp>

// int main()
// {
//     sf::Window window(sf::VideoMode(800, 600), "SFML Window");

//     // run the program as long as the window is open
//     while (window.isOpen())
//     {
//         // check all the window's events that were triggered since the last iteration of the loop
//         sf::Event event;
//         while (window.pollEvent(event))
//         {
//             // "close requested" event: we close the window
//             if (event.type == sf::Event::Closed)
//                 window.close();
//         }
//     }    

//     return 0;
// }

// TO RUN:

// g++ -I"C:\SFML-2.6.1\include" -c main.cpp -o main.o
// g++ main.o -o app.exe -L"C:\SFML-2.6.1\lib" -lsfml-graphics -lsfml-window -lsfml-system -lsfml-main -mwindows

// OR

// MinGW32-make compile
// MinGW32-make link

// OR

// MinGW32-make all