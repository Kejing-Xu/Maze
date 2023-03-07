/* Your code here! */
#include <vector>
#include "dsets.h"

DisjointSets::DisjointSets(){
    /*Nothing to do here*/
}

DisjointSets::~DisjointSets(){
    /*Nothing to do here*/
}

void DisjointSets::addelements (int num){
    for(int i=0;i<num;i++){
        dset.push_back(-1);
    }
}

int DisjointSets::find (int elem){
    if(dset[elem]<0)
    return elem;
    else
    return find(dset[elem]);
}

void DisjointSets::setunion (int a, int b){
    int rootA = find(a);
    int rootB = find(b);
    if(rootA!=rootB){
        int newSize=dset[rootA]+dset[rootB];
        if(dset[rootA]<dset[rootB]||(dset[rootA]==dset[rootB]&&rootA<rootB)){
            dset[rootB]=rootA;
            dset[rootA]=newSize;
        }
        else if(dset[rootB]<dset[rootA]||(dset[rootA]==dset[rootB]&&rootB<rootA)){
            dset[rootA]=rootB;
            dset[rootB]=newSize;
        }
    }
}

int DisjointSets::size (int elem){
    int ret;
    int root=find(elem);
    ret=-dset[root];
    return ret;
}