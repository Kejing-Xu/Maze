/* Your code here! */
#pragma once
#include <vector>
#include <cmath>
#include "dsets.h"

#include "cs225/PNG.h"
#include "cs225/HSLAPixel.h"

using std::vector;
using cs225::PNG;
using cs225::HSLAPixel;


class SquareMaze{
    public:

    SquareMaze();	
    void makeMaze (int width, int height);
    bool canTravel (int x, int y, int dir) const;
    void setWall(int x, int y, int dir, bool exists);
    vector<int> solveMaze();
    PNG * drawMaze() const;
    PNG * drawMazeWithSolution();


    private:

    int width_;
    int height_;

    /*DisjointSets of connected cells*/
    DisjointSets D;
    /*To check if all the cells are connected*/
    bool allConnected;

    vector< bool > right;
	vector< bool > down;

    int findX ( int a ) const;
    int findY ( int a ) const;
    int findCell ( int x, int y ) const;
    void turnRed(HSLAPixel & input);
};