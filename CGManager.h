#pragma once
#include "CellGrid.h"

using namespace std;

class CGManager {

    public:
        
        // Hard coding this for now
        int n_CG = 4;
        CellGrid CGArray[4];

        int n_bodies;
        vector<Body> bodyArray;

        // Constructors
        CGManager() {}
        CGManager(CellGrid _CG) : CGArray{_CG} {}
        CGManager(CellGrid _CG0, CellGrid _CG1, CellGrid _CG2, CellGrid _CG3) :
            CGArray{_CG0, _CG1, _CG2, _CG3} {}
        
        CGManager(CellGrid _CG0, CellGrid _CG1, CellGrid _CG2, CellGrid _CG3, Body _bodyArray[], int _n_bodies) :
            CGArray{_CG0, _CG1, _CG2, _CG3}, bodyArray(_bodyArray, _bodyArray + _n_bodies) {n_bodies = _n_bodies;}
        
        CGManager(CellGrid _CG, Body _bodyArray[], int _n_bodies) :
            CGArray{_CG}, bodyArray(_bodyArray, _bodyArray + _n_bodies) {n_bodies = _n_bodies;}

        void addBody(Body &body) {
            bodyArray.push_back(body);
            int CG_index = 0;
            for (CellGrid &CG : CGArray) {CG.initSampleBody(body, CG_index++);} 
        }

        void sampleBodiesAcrossLODS() {
            int CG_index = 0;
            for (CellGrid &CG : CGArray) {

                for (int i=0; i<bodyArray.size(); i++) {
                    bool deleteBody = CG.sampleBody(bodyArray[i], CG_index);
                    if (deleteBody) {
                        bodyArray.erase(bodyArray.begin() + i);
                    }    
                }

                for (Body &body : bodyArray) {
                    CG.sampleAcc(body, CG_index);
                    body.sprite.setPosition(body.r);
                }
                CG_index++;
            }            
        }
};
