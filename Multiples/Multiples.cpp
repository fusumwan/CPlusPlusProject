#include <vector>
#include <list>

#include <set>
#include <deque>
#include <stack>
#include <bitset>
#include <algorithm>
#include <functional>
#include <numeric>
#include <utility>
#include <sstream>

#include <iomanip>
#include <cstdio>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <tuple>
#include <iostream>
#include <climits>
#include <map>


#define fr(i,n)for(int i=0,i##e=n;i<i##e;++i)
#define ec(i,m)fr(i,m.size())
#define srt(v)sort(v.begin(),v.end())
using namespace std;

//g++ -std=c++98 Multiples.cpp -o Multiples
//./Multiples


/*

Problem (1) Multiples:
Initial thoughts:

Due to the question given a range of integers from min to max,

Given a range of integers from min to max (inclusive), It is determined that the number within the range can be divisible by the factor.

So I try to use recursive, modulo operator and dynamic programming to solve that problem. All the result will save in the tree.

Pseudo-Code:

class Multiples {
    public procedure number(int min, int max, int factor) {
        Node n(min,max,factor,0);
        n.BaseUpdate();
        DP dp;
        int r=dp.Solution(min,n);
        return r;
    endprocedure
endclass

Reflection:

Actually, this is my first time to use dynamic programming PSSD practice question, 

some problems are not easy solved by DP. 

I got a lot of error when the some tree nodes have be created.

I find that some nodes are duplicated and the first node should be updated before importing to tree function. 

So after updating the nodes index then the programming passed all the tests.



*/

class Node{
    int _i=0;
    int _min;
    int _max;
    int _factor;
    int _base=0;
public:
    Node(){
        _i=0;
        _min=0;
        _max=0;
        _factor=0;
        _base=0;
    }
    Node(int min,int max,int factor,int base){
        _min=min;
        _max=max;
        _factor=factor;
        
        _base=base;
    }
    int get(){
        if(_i<_min || _i>_max)
            return 0;
        return (_i % _factor==0)?1:0;;
    }
    void setMin(int min){
        _min=min;
    }
    int getMin(){
        return _min;
    }
    int getMax(){
        return _max;
    }
    int getFactor(){
        return _factor;
    }
    void setI(int i){
        _i=i;
    }
    void BaseUpdate(){
        _base+=get();
    }
    int getBase(){
        return _base;
    }
    bool end(){
        if(_i>_max)
            return true;
        return false;
    }
};

class DP{
public:
    
    std::map<int, Node> store;
    int Solution(int i,Node n){
        int len=n.getMax()-i;
        int def_len=1000;
        if(len<def_len && len>0){
            return DecisionTree(i,n).getBase();
        }
        else if(len<=0){
            if(n.getMax()%n.getFactor()==0){
                return 1;
            }
            return 0;
        }
        int t=len/def_len;
        int r=len%def_len;
        int count=0;
        int turn=0;
        for(int x=i;x<n.getMax();x++){
            Node nr(x,x,n.getFactor(),0);
            nr.setI(x);
            nr.BaseUpdate();
            store.clear();
            count+=DecisionTree(x,nr).getBase();
            turn+=1;
        }
        if(len%def_len==0 || def_len%len==0){
        	if(len%n.getFactor()==0)
                count+=1;
        }
        return count;
    }
    Node DecisionTree(int i,Node n) {
        if (n.end()){
            return n;
        }
        else if (store.find(i)!= store.end()) {
            return (Node) store[i];
        }else{
            Node next(n.getMin(),n.getMax(),n.getFactor(),n.getBase());
            next.setI(i+1);
            next.BaseUpdate();
            
            Node result = DecisionTree(i+1,next);
            store[i]=result;
            return result;
        }
    }
};


class Multiples {
public:
    int number(int min, int max, int factor) {
        Node n(min,max,factor,0);
        n.setI(min);
        n.BaseUpdate();
        DP dp;
        int r=dp.Solution(min,n);
        return r;
    }
};


double test0() {
    int p0 = 0;
    int p1 = 14;
    int p2 = 5;
    Multiples * obj = new Multiples();
    clock_t start = clock();
    int my_answer = obj->number(p0, p1, p2);
    clock_t end = clock();
    delete obj;
    cout <<"Time: " <<(double)(end-start)/CLOCKS_PER_SEC <<" seconds" <<endl;
    int p3 = 3;
    cout <<"Desired answer: " <<endl;
    cout <<"\t" << p3 <<endl;
    cout <<"Your answer: " <<endl;
    cout <<"\t" << my_answer <<endl;
    if (p3 != my_answer) {
        cout <<"DOESN'T MATCH!!!!" <<endl <<endl;
        return -1;
    }
    else {
        cout <<"Match :-)" <<endl <<endl;
        return (double)(end-start)/CLOCKS_PER_SEC;
    }
}
double test1() {
    int p0 = 7;
    int p1 = 24;
    int p2 = 3;
    Multiples * obj = new Multiples();
    clock_t start = clock();
    int my_answer = obj->number(p0, p1, p2);
    clock_t end = clock();
    delete obj;
    cout <<"Time: " <<(double)(end-start)/CLOCKS_PER_SEC <<" seconds" <<endl;
    int p3 = 6;
    cout <<"Desired answer: " <<endl;
    cout <<"\t" << p3 <<endl;
    cout <<"Your answer: " <<endl;
    cout <<"\t" << my_answer <<endl;
    if (p3 != my_answer) {
        cout <<"DOESN'T MATCH!!!!" <<endl <<endl;
        return -1;
    }
    else {
        cout <<"Match :-)" <<endl <<endl;
        return (double)(end-start)/CLOCKS_PER_SEC;
    }
}
double test2() {
    int p0 = -123456;
    int p1 = 654321;
    int p2 = 997;
    Multiples * obj = new Multiples();
    clock_t start = clock();
    int my_answer = obj->number(p0, p1, p2);
    clock_t end = clock();
    delete obj;
    cout <<"Time: " <<(double)(end-start)/CLOCKS_PER_SEC <<" seconds" <<endl;
    int p3 = 780;
    cout <<"Desired answer: " <<endl;
    cout <<"\t" << p3 <<endl;
    cout <<"Your answer: " <<endl;
    cout <<"\t" << my_answer <<endl;
    if (p3 != my_answer) {
        cout <<"DOESN'T MATCH!!!!" <<endl <<endl;
        return -1;
    }
    else {
        cout <<"Match :-)" <<endl <<endl;
        return (double)(end-start)/CLOCKS_PER_SEC;
    }
}
double test3() {
    int p0 = -75312;
    int p1 = 407891;
    int p2 = 14;
    Multiples * obj = new Multiples();
    clock_t start = clock();
    int my_answer = obj->number(p0, p1, p2);
    clock_t end = clock();
    delete obj;
    cout <<"Time: " <<(double)(end-start)/CLOCKS_PER_SEC <<" seconds" <<endl;
    int p3 = 34515;
    cout <<"Desired answer: " <<endl;
    cout <<"\t" << p3 <<endl;
    cout <<"Your answer: " <<endl;
    cout <<"\t" << my_answer <<endl;
    if (p3 != my_answer) {
        cout <<"DOESN'T MATCH!!!!" <<endl <<endl;
        return -1;
    }
    else {
        cout <<"Match :-)" <<endl <<endl;
        return (double)(end-start)/CLOCKS_PER_SEC;
    }
}

int main() {

    int min=1000000;
    int max=1000000;
    int factor=1;
    Multiples * obj = new Multiples();
    cout<<obj->number(min, max, factor)<<endl;
    int c = 0;
    for (int i = min; i <= max; ++i){
        if (i % factor == 0){
            ++c;
        }
    }
    cout<<c<<endl;

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

    if (!errors)
        cout <<"You're a stud (at least on the example cases)!" <<endl;
    else
        cout <<"Some of the test cases had errors." <<endl;
*/
}
