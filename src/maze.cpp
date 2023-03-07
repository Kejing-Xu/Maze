/* Your code here! */
#include <algorithm>
#include <vector>
#include <queue>
#include <cstdlib>
#include <sys/time.h>
#include <time.h>
#include <cmath>
#include <unistd.h>
#include "maze.h"
#include "dsets.h"

SquareMaze::SquareMaze(){
    /*Nothing to do here*/
}

void SquareMaze::makeMaze (int width, int height){
    width_=width;
    height_=height;
    if(!right.empty()){
        right.clear();
    }
    if(!down.empty()){
        down.clear();
    }
    for(int i=0;i<width_*height_;i++){
        right.push_back(true);
        down.push_back(true);
    }
    D.addelements(width_*height_);
    /*Initialize random number*/
    srand ( time(NULL) );
    allConnected=false;
    int edges=0;
    /*randomly delete walls, until the edges to be sum of cells-1*/
    while(edges!=width_*height_-1){
        int cellPos=rand()%(width_*height_);
        int goFind=rand()%2;
        if(goFind==0&&findX(cellPos)<width_-1){
            if ( D.find(cellPos) != D.find(cellPos+1 ) ){
                setWall(findX(cellPos),findY(cellPos),0,false);
                D.setunion(cellPos,cellPos+1);
                edges++;
            }
        }
        if(goFind==1&&findY(cellPos)<height_-1){
            if ( D.find(cellPos) != D.find(cellPos+width_) ){
                setWall(findX(cellPos),findY(cellPos),1,false);
                D.setunion(cellPos,cellPos+width_);
                edges++;
            }
        }
    }
    allConnected=true;
    sleep(1);
}

bool SquareMaze::canTravel (int x, int y, int dir) const{
    if(dir==2&&x==0)
    return false;
    if(dir==3&&y==0)
    return false;
    int cell=findCell(x,y);
    int cellLeft,cellUp;
    if(x!=0)
    cellLeft=findCell(x-1,y);
    if(y!=0)
    cellUp=findCell(x,y-1);
    if(dir==0&&right[cell]==false)
    return true;
    else if(dir==1&&down[cell]==false)
    return true;
    else if(dir==2&&right[cellLeft]==false)
    return true;
    else if(dir==3&&down[cellUp]==false)
    return true;
    else
    return false;
}

void SquareMaze::setWall(int x, int y, int dir, bool exists){
    if(dir==0){
        if(exists==true)
        right[findCell(x,y)]=true;
        else
        right[findCell(x,y)]=false;
    }
    else{
        if(exists==true)
        down[findCell(x,y)]=true;
        else
        down[findCell(x,y)]=false;
    }
}

vector<int> SquareMaze::solveMaze(){
    /*using BFS*/
   vector<int> distance(width_*height_,-1);
    queue<int> q;
    vector<bool> visited(width_*height_,false);
    /*solutions of traversal to each cell*/
    vector<vector<int>> solutions(width_*height_,vector<int>());
    int start = 0;
    distance[start]=0;
    visited[start]=true;
    q.push(start);
    while(!q.empty()){
        int cell=q.front();
        q.pop();
        if(canTravel(findX(cell),findY(cell),0)&&visited[cell+1]==false){
        q.push(cell+1);
        distance[cell+1]=distance[cell]+1;
        visited[cell+1]=true;
        solutions[cell+1]=solutions[cell];
        solutions[cell+1].push_back(0);
        }
        if(canTravel(findX(cell),findY(cell),1)&&visited[cell+width_]==false){
        q.push(cell+width_);
        distance[cell+width_]=distance[cell]+1;
        visited[cell+width_]=true;
        solutions[cell+width_]=solutions[cell];
        solutions[cell+width_].push_back(1);
        }
        if(canTravel(findX(cell),findY(cell),2)&&visited[cell-1]==false){
        q.push(cell-1);
        distance[cell-1]=distance[cell]+1;
        visited[cell-1]=true;
        solutions[cell-1]=solutions[cell];
        solutions[cell-1].push_back(2);
        }
        if(canTravel(findX(cell),findY(cell),3)&&visited[cell-width_]==false){
        q.push(cell-width_);
        distance[cell-width_]=distance[cell]+1;
        visited[cell-width_]=true;
        solutions[cell-width_]=solutions[cell];
        solutions[cell-width_].push_back(3);
        }
    }
    /*compare distances of bottom row*/
    int maxX=-1;
    int y=height_-1;
    int maxDistance=0;
    for(int x=0;x<width_;x++){
        int cellBottom=findCell(x,y);
        if(distance[cellBottom]>maxDistance){
            maxX=x;
            maxDistance=distance[cellBottom];
        }
    }
    return solutions[findCell(maxX,y)];
}

PNG * SquareMaze::drawMaze() const{
    int widthPNG=(width_*10)+1;
    int heightPNG=(height_*10)+1;
    PNG* maze = new PNG(widthPNG,heightPNG);
    for(int i=10;i<widthPNG;i++){
        maze->getPixel(i,0).l=0;
    }
    for(int j=0;j<heightPNG;j++){
        maze->getPixel(0,j).l=0;
    }
    int k=0;
    for(int x=0;x<width_;x++){
        for(int y=0;y<height_;y++){
            if(right[findCell(x,y)]==true){
                for(k=0;k<11;k++)
                maze->getPixel((x+1)*10,y*10+k).l=0;
            }
            if(down[findCell(x,y)]==true){
                for(k=0;k<11;k++)
                maze->getPixel(x*10+k, (y+1)*10).l=0;
            }
        }
    }
    return maze;
}

PNG * SquareMaze::drawMazeWithSolution(){
    PNG * maze = drawMaze();
    int start=0;
    int mazeX=findX(start);
    int mazeY=findY(start);
    int x=5,y=5;
    auto solution=solveMaze();
    turnRed(maze->getPixel(5,5));
    for(int dir : solution){
        int i=1;
        if(dir==0){
            for(;i<=10;i++)
            turnRed(maze->getPixel(x+i,y));
            x=x+10;
            mazeX++;
        }
        else if(dir==1){
            for(;i<=10;i++)
            turnRed(maze->getPixel(x,y+i));
            y=y+10;
            mazeY++;
        }
        else if(dir==2){
            for(;i<=10;i++)
            turnRed(maze->getPixel(x-i,y));
            x=x-10;
            mazeX--;
        }
        else if(dir==3){
            for(;i<=10;i++)
            turnRed(maze->getPixel(x,y-i));
            y=y-10;
            mazeY--;
        }
    }
    for(int k=1;k<=9;k++)
    maze->getPixel(mazeX*10+k, (mazeY+1)*10).l=1;
    return maze;
}

int SquareMaze::findX ( int a ) const{
    return a%width_;
}

int SquareMaze::findY ( int a ) const{
    return int(a/width_);
}

int SquareMaze::findCell ( int x, int y ) const{
    return y*width_+x;
}

void SquareMaze::turnRed(HSLAPixel & input){
    input.h=0;
    input.s=1;
    input.l=0.5;
    input.a=1;
}