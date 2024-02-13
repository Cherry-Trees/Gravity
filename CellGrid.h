#pragma once
#include <math.h>
#include <vector>
#include <map>
#include <iostream>
#include "Cell.h"
using namespace std;

#define G 6.67e-11f

class CellGrid {

    public:
        int shape[2];
        int area;
        int xlim[2], ylim[2];
        float delta_x, delta_y;

        CellGrid() {}
        CellGrid(int _shape[2], int _xlim[2], int _ylim[2]) : 
            shape{_shape[0], _shape[1]}, 
            xlim{_xlim[0], _xlim[1]},
            ylim{_ylim[0], _ylim[1]} 
            {
                area = shape[1] * shape[0];
                delta_x = (xlim[1] - xlim[0]) / static_cast<float>(shape[0]);
                delta_y = (ylim[1] - ylim[0]) / static_cast<float>(shape[1]);

                // Construct user-defined 2D arrays for Cells and hashes
                cellArray2 = new Cell*[shape[1]];
                hashArray2 = new int*[shape[1]];
                for (int i=0; i<shape[1]; i++) 
                {
                    cellArray2[i] = new Cell[shape[0]];
                    hashArray2[i] = new int[shape[0]];
                }

                // Fill arrays with Cells and hashes
                for (int i=0; i<shape[1]; i++) {
                    for (int j=0; j<shape[0]; j++) {

                        cellArray2[i][j] = Cell(xlim[0] + delta_x*j, xlim[0] + delta_x*(j+1),
                                                ylim[1] - delta_y*i, ylim[1] - delta_y*(i+1),
                                                j, i);
                        
                        hashArray2[i][j] = cellArray2[i][j].getHash();
                    }
                }

                // Finally, build the hash table
                for (int i=0; i<area; i++) {
                    cellHashMap[hashArray2[i/shape[1]][i%shape[0]]] = cellArray2[i/shape[1]][i%shape[0]];
                    //cout << "HASH: " << hashArray2[i/shape[1]][i%shape[0]] << " --> CELL: " <<  cellArray2[i/shape[1]][i%shape[0]] << endl;
                }
            }

            Cell& getCellFromHash(int _hash) {return cellHashMap[_hash];}
            Cell& getCellFromIndices(int _i, int _j) {return cellArray2[_i][_j];}
            int getHashFromIndices(int _i, int _j) {return hashArray2[_i][_j];}

            int* getArrayOfCellHashesInRadius(int _centerCellHash, int _RADIUS);
            bool sampleBody(Body &body, int _CG_index);
            void sampleAcc(Body &body, int _CG_index, const float _DELTA);

            void initSampleBody(Body &body, int _CG_index) {
                body.setCurrentCellHash(xlim, ylim, delta_x, delta_y, _CG_index);
                cellHashMap[body.getCurrentCellHash(_CG_index)].addBody(body);
            }

        private:
            Cell** cellArray2;
            int** hashArray2;
            map<int, Cell> cellHashMap;

            

};

ostream& operator<<(ostream &out, CellGrid &cg) {
    for (int i=0; i<cg.shape[1]; i++) {
        for (int j=0; j<cg.shape[0]; j++) {
            out << cg.getCellFromIndices(i, j) << endl;
        }
        out << endl;
    }
    return out;
}

int* CellGrid::getArrayOfCellHashesInRadius(int _centerCellHash, const int _RADIUS=5) {
    int startingXIndex = cellHashMap[_centerCellHash].getXIndex();
    int startingYIndex = cellHashMap[_centerCellHash].getYIndex();

    int minXIndex = max(startingXIndex - _RADIUS, 0);
    int minYIndex = max(startingYIndex - _RADIUS, 0);
    int maxXIndex = min(startingXIndex + _RADIUS, shape[0] - 1);
    int maxYIndex = min(startingYIndex + _RADIUS, shape[1] - 1);

    int* surroundingCellHashArray = new int[((maxYIndex - minYIndex)+1) * ((maxXIndex - minXIndex)+1) - 1 + 1];
    int iters = 0;

    // Fill array with surrounding Cell hashes
    for (int i=minYIndex; i<maxYIndex+1; i++) {
        for (int j=minXIndex; j<maxXIndex+1; j++) {
            if (hashArray2[i][j] != _centerCellHash) {
                surroundingCellHashArray[1+iters++] = hashArray2[i][j];     
            }    
        }
    }

    // First element in array is its length
    surroundingCellHashArray[0] = iters;
    return surroundingCellHashArray;
}

bool CellGrid::sampleBody(Body &body, int _CG_index) {
    int prev_hash = body.getCurrentCellHash(_CG_index);
    body.setCurrentCellHash(xlim, ylim, delta_x, delta_y, _CG_index);
    int current_hash = body.getCurrentCellHash(_CG_index);
    bool deleteBody = false;

    //cellHashMap[body.getCurrentCellHash(_CG_index)].addBody(body);

    if (cellHashMap.find(current_hash) == cellHashMap.end())
        deleteBody = true;
    
    else {
        deleteBody = false;
        if (current_hash != prev_hash) {
            cellHashMap[prev_hash].removeBody(body);
            cellHashMap[current_hash].addBody(body);
        }
    }
    return deleteBody;


    // if (current_hash != prev_hash) {
    //     cellHashMap[prev_hash].removeBody(body);
    //     cellHashMap[current_hash].addBody(body);
    // }
}

void CellGrid::sampleAcc(Body &body, int _CG_index, const float _DELTA=0.01f) {
    int currentBodyCellHash = body.getCurrentCellHash(_CG_index);
    int* surroundingCellHashArray = getArrayOfCellHashesInRadius(currentBodyCellHash, 3);
    int n_surroundingCells = surroundingCellHashArray[0];

    for (int i=1; i<n_surroundingCells+1; i++) {
        Cell interactingCell = cellHashMap[*(surroundingCellHashArray+i)];
        body.a[_CG_index] = (
                (0.01f * interactingCell.getTotalMass() * interactingCell.distanceFromBodyVectorized(body)) / 
                (interactingCell.distanceFromBody(body)*interactingCell.distanceFromBody(body)*interactingCell.distanceFromBody(body))
            );
        
        // Temporary Euler integration
        body.v += body.a[_CG_index] * _DELTA;
        body.r += body.v * _DELTA;
    }
}

// DELETE BODIES OUTSIDE OF RANGE