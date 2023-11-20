#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <utility>
#include <set>
#include <cctype>
#include <queue>
#include <stack>
using namespace std;

//g++ -std=c++11 CellRemoval.hpp -o CellRemoval
//./CellRemoval





class CellRemoval{
public:
    int deleteC;
    vector<int> cellstore[100];
    int count;
    vector <int> pcells;
    int celldel;
    int selected;
    vector <int> ccells;
    int del(int cell){
        if (cell == -1) {
            count++;
            return 0;
        } else if (cell == celldel) {
            return 0;
        }
        del(pcells[cell]);
        return 0;
    }
    void setup_lefts(){
        int i=0;
        while(i<pcells.size()){
            ccells.push_back(0);
            i+=1;
        }
    }
    void count_lefts(){
        int i=0;
        while(i<pcells.size()){
            if (ccells[i] == 0){
                del(i);
            }
        }
    }
    void remove(){
        int i=0;
        while(i<pcells.size()){
            if (pcells[i] != -1){
                ccells[pcells[i]]++;
            }
        }
    }

    int regis;
    void process(int s) {
        if(s==deleteC){
            return;
        }
        vector<int>::iterator i=cellstore[s].begin();
        while(i!=cellstore[s].end()){
            process(*i);
            i+=1;
        }
        if(0==cellstore[s].size()) ++regis;
    }
    int cellsLeft(vector <int> parent, int deletedCell) {
        pcells.clear();
        ccells.clear();


        deleteC=deletedCell;
        celldel = deletedCell;
        regis=0;
        count=0;
        for(int i=0;i<100;i++){
            cellstore[i].clear();
        }
        
        setup_lefts();
        int i=0;
        while(i<parent.size()){
            if(-1!=parent[i]) {
                cellstore[parent[i]].push_back(i);
            }else{
                selected=i;
            }
            i+=1;
        }
        count_lefts();
        process(selected);
        remove();
        if(regis>count){
            if(count%regis==0){
                return regis;
            }
        }
        else if(count==regis){
            return count;
        }
        return regis;
    }
};

