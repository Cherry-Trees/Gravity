#pragma once
#include <math.h>
#include <vector>
#include <functional>
#include "functions.h"
#include "Body.h"
using namespace std;

/*
(X0,Y0)----------(X,Y0)
|                     |
|                     |
|                     |
|                     |
|                     |
|                     |
|                     |
(X0,Y)------------(X,Y)
*/
class Cell {
    
    public:
        float x0, x, y0, y;
        int indices[2];       

        Cell() {}
        Cell(float x0, float x, float y0, float y, int index_x, int index_y) {
            this->x0 = x0;
            this->x = x;
            this->y0 = y0;
            this->y = y;
            this->index_x = index_x;
            this->index_y = index_y;

            this->center = Vector2f((x + x0) / 2.0, (y + y0) / 2.0);
            this->totalMass = 0;
            this->setHash();
        }

        int getHash() {return hash;}
        int getXIndex() {return index_x;}
        int getYIndex() {return index_y;}
        int getTotalMass() {return totalMass;}

        Vector2f distanceFromBodyVectorized(Body body) {return center - body.r;}
        float distanceFromBody(Body body) {
            return sqrt((body.r.x - center.x)*(body.r.x - center.x) + (body.r.y - center.y)*(body.r.y - center.y));
        }

        void addBody(Body body) {totalMass += body.m;} //currentBodies.push_back(body);}
        void removeBody(Body body) {totalMass -= body.m;}
    
    private:
        Vector2f center;
        float totalMass;
        int hash;
        int index_x, index_y;
        void setHash() {hash = rangeHash(x0, x, y0, y);}// cout << "TEST: " << x0 << " " << x << " " << y0 << " " << y << " " << endl;}
        
};


ostream& operator<<(ostream &out, Cell &cell) {
    return out << "Cell[ [" << cell.x0 << " " << cell.x << "] [" << cell.y0 << " " << cell.y << "] ] ";
}
