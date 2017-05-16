#include <iostream>
#include <fstream>
#include <stdlib.h>
#include<time.h>
#define inf 1001

using namespace std;


int main(){

    int **dist,n;
    int matrixSize=701;

    dist=(int **) malloc(matrixSize*sizeof(int *));

    for(int i=0;i<matrixSize;i++){
        dist[i]=(int *) malloc(matrixSize*sizeof(int));
    }

    ifstream in("royfloyd.in");

    in>>n;
    for(int i=1;i<=n;++i){
        for(int j=1;j<=n;++j){
            in>>dist[i][j];
            if(dist[i][j]==0 && i!=j)
                dist[i][j]=inf;
        }
    }
    clock_t t;
    t = clock();
    for(int k1=1;k1<=1;++k1){
        for(int k=1;k<=n;++k){
            for(int i=1;i<=n;++i){
                for(int j=1;j<=n;++j){
                    if(dist[i][j]>dist[i][k]+dist[k][j] && dist[i][k]!=inf && dist[k][j]!=inf && i!=k && j!=k){
                        dist[i][j]=dist[i][k]+dist[k][j];
                    }
                }
            }
        }
    }

    t = clock() - t;

   cout<<" execution time: "<<((double)t)/CLOCKS_PER_SEC<<endl;



    return 0;
}
