#pragma once
#include <SFML/Graphics.hpp>
#include <math.h>
#include <iostream>


using namespace sf;

class Body {

    public:

        float m;
        Vector2f r;
        Vector2f v;
        Vector2f a[4];
        CircleShape sprite;
        float size = 5;

        Body() {}

        Body(float m, float x0, float y0, float vx0, float vy0) : r{x0, y0}, v{vx0, vy0} {
            this->m = m;
            this->sprite = CircleShape(size);
            this->sprite.setFillColor(Color::White);
        }

        Body(float m, float x0, float y0) : r{x0, y0}, v{0, 0} {
            this->m = m;
            this->sprite = CircleShape(size);
            this->sprite.setFillColor(Color::White);
        }

        float X() {return r.x;}
        float Y() {return r.y;}

        int getCurrentCellHash(int _CG_index) {return currentCellHashes[_CG_index];}
        void setCurrentCellHash(int _xlim[2], int _ylim[2], float _cell_delta_x, float _cell_delta_y, int _CG_index) {
            
            // Set Cell scanner to top left
            float scanner_x = _xlim[0];
            float scanner_y = _ylim[0];

            // Scan until we reach the Body's current position
            while (r.x > scanner_x) {scanner_x += _cell_delta_x;}
            while (r.y > scanner_y) {scanner_y += _cell_delta_y;}

            currentCellHashes[_CG_index] = rangeHash(scanner_x - _cell_delta_x, scanner_x,
                                                    scanner_y, scanner_y - _cell_delta_y);
        }
    
    private:

        // Hard coding this for now
        int currentCellHashes[4];
        
};

ostream& operator<<(ostream &out, Body &body) {
    return out << "Body[ " << body.X() << ", " << body.Y() << " ] ";  
}
