#include <vector>
#include <list>
#include <map>
#include <set>
#include <queue>
#include <deque>
#include <stack>
#include <bitset>
#include <algorithm>
#include <functional>
#include <numeric>
#include <utility>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <cstdio>
#include <cstring>
#include <cmath>
#include <cstdlib>
#include <ctime>
#define x first
#define y second
#define LL long long
using namespace std;

typedef pair<int, int> per;
typedef vector<int>::iterator it;
typedef set<int>::iterator is;
//g++ -std=c++11 CellRemoval.cpp -o CellRemoval
//./CellRemoval


/*
Problem (1) CellRemoval:

Initial thoughts:

According to the question, everytime will start from the first cell, 

So there will be exactly one "-1" element in parentCell. 

In order to make each cell in the development of the organism either divides 

into two other cells or does not divide at all. We need to two arrays, one is for parent cell and other is for child cell.

for example if the parent cells are:

parent{0,1,2,3}

and the child cells are:

child{-1,0,0,3}

child[1] and child[2] are inherited from parent[0];


Pseudo-Code:
class CellRemoval {
    public procedure cellsLeft(vector <int> parent, int deletedCell) {
        count = 0;
        pcells = parent;
        celldel = deletedCell;
        setup_lefts();
        count_lefts();
        remove();
        return count;
    endprocedure
endclass

Reflection:

I forgot something. When the system is running, 

it will use the previously recorded data for the next run. 

In fact, I should reset the program into initialization.

I would like to choose brute force. 

Because I want to solves a problem in the most simple, direct or obvious way. 

And there is not distinguished by structure or form in the instructions.

It just simple to make each cell in the development of the organism either divides 

into two other cells. Hence, this program is mainly used by two array to cater all the inheritance logic.


*/


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


double test0() {
    int t0[] = {-1,0,0,1,1};
    vector <int> p0(t0, t0+sizeof(t0)/sizeof(int));
    int p1 = 2;
    CellRemoval * obj = new CellRemoval();
    clock_t start = clock();
    int my_answer = obj->cellsLeft(p0, p1);
    clock_t end = clock();
    delete obj;
    cout <<"Time: " <<(double)(end-start)/CLOCKS_PER_SEC <<" seconds" <<endl;
    int p2 = 2;
    cout <<"Desired answer: " <<endl;
    cout <<"\t" << p2 <<endl;
    cout <<"Your answer: " <<endl;
    cout <<"\t" << my_answer <<endl;
    if (p2 != my_answer) {
        cout <<"DOESN'T MATCH!!!!" <<endl <<endl;
        return -1;
    }
    else {
        cout <<"Match :-)" <<endl <<endl;
        return (double)(end-start)/CLOCKS_PER_SEC;
    }
}
double test1() {
    int t0[] = {-1,0,0,1,1};
    vector <int> p0(t0, t0+sizeof(t0)/sizeof(int));
    int p1 = 1;
    CellRemoval * obj = new CellRemoval();
    clock_t start = clock();
    int my_answer = obj->cellsLeft(p0, p1);
    clock_t end = clock();
    delete obj;
    cout <<"Time: " <<(double)(end-start)/CLOCKS_PER_SEC <<" seconds" <<endl;
    int p2 = 1;
    cout <<"Desired answer: " <<endl;
    cout <<"\t" << p2 <<endl;
    cout <<"Your answer: " <<endl;
    cout <<"\t" << my_answer <<endl;
    if (p2 != my_answer) {
        cout <<"DOESN'T MATCH!!!!" <<endl <<endl;
        return -1;
    }
    else {
        cout <<"Match :-)" <<endl <<endl;
        return (double)(end-start)/CLOCKS_PER_SEC;
    }
}
double test2() {
    int t0[] = {-1,0,0,1,1};
    vector <int> p0(t0, t0+sizeof(t0)/sizeof(int));
    int p1 = 0;
    CellRemoval * obj = new CellRemoval();
    clock_t start = clock();
    int my_answer = obj->cellsLeft(p0, p1);
    clock_t end = clock();
    delete obj;
    cout <<"Time: " <<(double)(end-start)/CLOCKS_PER_SEC <<" seconds" <<endl;
    int p2 = 0;
    cout <<"Desired answer: " <<endl;
    cout <<"\t" << p2 <<endl;
    cout <<"Your answer: " <<endl;
    cout <<"\t" << my_answer <<endl;
    if (p2 != my_answer) {
        cout <<"DOESN'T MATCH!!!!" <<endl <<endl;
        return -1;
    }
    else {
        cout <<"Match :-)" <<endl <<endl;
        return (double)(end-start)/CLOCKS_PER_SEC;
    }
}
double test3() {
    int t0[] = {-1,0,0,2,2,4,4,6,6};
    vector <int> p0(t0, t0+sizeof(t0)/sizeof(int));
    int p1 = 4;
    CellRemoval * obj = new CellRemoval();
    clock_t start = clock();
    int my_answer = obj->cellsLeft(p0, p1);
    clock_t end = clock();
    delete obj;
    cout <<"Time: " <<(double)(end-start)/CLOCKS_PER_SEC <<" seconds" <<endl;
    int p2 = 2;
    cout <<"Desired answer: " <<endl;
    cout <<"\t" << p2 <<endl;
    cout <<"Your answer: " <<endl;
    cout <<"\t" << my_answer <<endl;
    if (p2 != my_answer) {
        cout <<"DOESN'T MATCH!!!!" <<endl <<endl;
        return -1;
    }
    else {
        cout <<"Match :-)" <<endl <<endl;
        return (double)(end-start)/CLOCKS_PER_SEC;
    }
}
double test4() {
    int t0[] = {26,2,32,36,40,19,43,24,30,13,21,14,24,21,19,4,30,10,44,12,7,32,17,43,
 35,18,7,36,10,16,5,38,35,4,13,-1,16,26,1,12,2,5,18,40,1,17,38,44,14};
    vector <int> p0(t0, t0+sizeof(t0)/sizeof(int));
    int p1 = 24;
    CellRemoval * obj = new CellRemoval();
    clock_t start = clock();
    int my_answer = obj->cellsLeft(p0, p1);
    clock_t end = clock();
    delete obj;
    cout <<"Time: " <<(double)(end-start)/CLOCKS_PER_SEC <<" seconds" <<endl;
    int p2 = 14;
    cout <<"Desired answer: " <<endl;
    cout <<"\t" << p2 <<endl;
    cout <<"Your answer: " <<endl;
    cout <<"\t" << my_answer <<endl;
    if (p2 != my_answer) {
        cout <<"DOESN'T MATCH!!!!" <<endl <<endl;
        return -1;
    }
    else {
        cout <<"Match :-)" <<endl <<endl;
        return (double)(end-start)/CLOCKS_PER_SEC;
    }
}

int main() {
    /*
    int time;
    bool errors = false;
    
    time = test0();
    if (time < 0)
        errors = true;
    
    time = test1();
    if (time < 0)
        errors = true;
    
    time = test2();
    if (time < 0)
        errors = true;
    
    time = test3();
    if (time < 0)
        errors = true;
    
    time = test4();
    if (time < 0)
        errors = true;
    
    if (!errors)
        cout <<"You're a stud (at least on the example cases)!" <<endl;
    else
        cout <<"Some of the test cases had errors." <<endl;

    */

    CellRemoval c;
    std::cout<<c.cellsLeft({-1,0,0}, 0)<<std::endl;
    std::cout<<c.cellsLeft({-1,0,0}, 1)<<std::endl;
    
}
