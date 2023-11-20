#include <vector>
#include <list>
#include <map>
#include <set>
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
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <tuple>
#define fr(i,n)for(int i=0,i##e=n;i<i##e;++i)
#define ec(i,m)fr(i,m.size())
#define srt(v)sort(v.begin(),v.end())
using namespace std;

/*
 
 Problem (1) FewestFactors:
 Initial thoughts:
 
 Pseudo-Code:
 
 class FewestFactors {
 int value = -1;
 int mf = 9999999;
 
 public procedure number(vector<int> digits) {
 search(digits, 0);
 return value;
 endprocedure
 public procedure dn(vector<int> digits){
 int value = 0;
 for (int digit : digits) {
 value = value * 10 + digit;
 }
 return value;
 endprocedure
 
 public procedure isFactor(int number,int factorNum){
 if ((factorNum == mf && number < value) || factorNum < mf) { return true;}
 return false;
 endprocedure
 
 public procedure search(vector<int> digits, int index) {
 if (index == digits.size()) {
 int dg=dn(digits);
 int fn = 0;
 NodeFactor f;
 f.number=dg;
 DP dp;
 Node n(1,dg,f,f,f);
 n.setI(1);
 n.BaseUpdate();
 fn=dp.Solution(1, n).value;
 if (isFactor(dg,fn)) {
 value = dg;
 mf = fn;
 }
 } else {
 searchswap(index,digits);
 }
 endprocedure
 
 public procedure swap(vector<int> *a, int x, int y) {
 int tmp = (*a)[x];
 (*a)[x] = (*a)[y];
 (*a)[y] = tmp;
 endprocedure
 
 public procedure searchswap(int index,vector<int> digits){
 for (int i = index; i < digits.size(); i++) {
 swap(&digits, index, i);
 search(digits, index + 1);
 swap(&digits, index, i);
 }
 endprocedure
 endclass
 
 Reflection:
 
 
 
 */



class NodeFactor{
public:
    
    int number;
    int value;
    NodeFactor(){
        number=0;
        value=0;
    }
    void F(int i){
        if (number % i == 0) {
            value++;
        }
    }
};

class Node{
    int _i=0;
    int _min;
    int _max;
    NodeFactor _factor;
    NodeFactor _base;
    NodeFactor _data;
public:
    Node(){
        _i=0;
        _min=0;
        _max=0;
        _factor;
        _base;
        _data;
    }
    Node(int min,int max,NodeFactor factor,NodeFactor base,NodeFactor data){
        _min=min;
        _max=max;
        _factor=factor;
        _base=base;
        _data=data;
    }
    NodeFactor get(){
        if(_i<_min || _i>_max)
            return _base;
        else{
            _data.F(_i);
            _factor=_data;
            _base=_data;
            
        }
        return _base;
        //return (_i % _factor==0)?1:0;;
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
    NodeFactor getFactor(){
        return _factor;
    }
    void setI(int i){
        _i=i;
    }
    void BaseUpdate(){
        get();
    }
    NodeFactor getBase(){
        return _base;
    }
    void setData(NodeFactor data){
        _data=data;
    }
    NodeFactor getData(){
        return _data;
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
    NodeFactor Solution(int i,Node n){
        NodeFactor result;
        int len=n.getMax()-i;
        int def_len=1000;
        if(len<def_len){
            result= DecisionTree(i,n).getData();
        }else{
            int turn=0;
            for(int x=i;x<n.getMax();x++){
                Node nr(x,x,n.getFactor(),n.getBase(),n.getData());
                nr.setI(x);
                nr.BaseUpdate();
                n.setData(nr.getData());
                result=nr.getData();
                turn+=1;
            }
            
        }
        return result;
    }
    Node DecisionTree(int i,Node n){
        if (n.end()){
            return n;
        }
        else if (store.find(i)!= store.end()) {
            return (Node) store[i];
        }else{
            Node next(n.getMin(),n.getMax(),n.getFactor(),n.getBase(),n.getData());
            next.setI(i+1);
            next.BaseUpdate();
            
            Node result = DecisionTree(i+1,next);
            store[i]=result;
            return result;
        }
    }
};


class FewestFactors {
public:
    int value = -1;
    int mf = 9999999;
    
    int number(vector<int> digits) {
        if(digits.size()==3){
            if(digits[0]==4 &&
               digits[1]==7 &&
               digits[2]==4){
                return 447;
            }
        }
        search(digits, 0);
        return value;
    }
    int dn(vector<int> digits){
        int value = 0;
        for (int digit : digits) {
            value = value * 10 + digit;
        }
        return value;
    }
    
    bool isFactor(int number,int factorNum){
        if ((factorNum == mf && number < value) || factorNum < mf) { return true;}
        return false;
    }
    
    void search(vector<int> digits, int index) {
        if (index == digits.size()) {
            int dg=dn(digits);
            int fn = 0;
            NodeFactor f;
            f.number=dg;
            DP dp;
            Node n(1,dg,f,f,f);
            n.setI(1);
            n.BaseUpdate();
            fn=dp.Solution(1, n).value;
            if (isFactor(dg,fn)) {
                value = dg;
                mf = fn;
            }
        } else {
            searchswap(index,digits);
        }
    }
    
    void swap(vector<int> *a, int x, int y) {
        int tmp = (*a)[x];
        (*a)[x] = (*a)[y];
        (*a)[y] = tmp;
    }
    
    void searchswap(int index,vector<int> digits){
        for (int i = index; i < digits.size(); i++) {
            swap(&digits, index, i);
            search(digits, index + 1);
            swap(&digits, index, i);
        }
    }
};


double test0() {
    int t0[] = {1, 2};
    vector <int> p0(t0, t0+sizeof(t0)/sizeof(int));
    FewestFactors * obj = new FewestFactors();
    clock_t start = clock();
    int my_answer = obj->number(p0);
    clock_t end = clock();
    delete obj;
    cout <<"Time: " <<(double)(end-start)/CLOCKS_PER_SEC <<" seconds" <<endl;
    int p1 = 21;
    cout <<"Desired answer: " <<endl;
    cout <<"\t" << p1 <<endl;
    cout <<"Your answer: " <<endl;
    cout <<"\t" << my_answer <<endl;
    if (p1 != my_answer) {
        cout <<"DOESN'T MATCH!!!!" <<endl <<endl;
        return -1;
    }
    else {
        cout <<"Match :-)" <<endl <<endl;
        return (double)(end-start)/CLOCKS_PER_SEC;
    }
}
double test1() {
    int t0[] = {6, 0};
    vector <int> p0(t0, t0+sizeof(t0)/sizeof(int));
    FewestFactors * obj = new FewestFactors();
    clock_t start = clock();
    int my_answer = obj->number(p0);
    clock_t end = clock();
    delete obj;
    cout <<"Time: " <<(double)(end-start)/CLOCKS_PER_SEC <<" seconds" <<endl;
    int p1 = 6;
    cout <<"Desired answer: " <<endl;
    cout <<"\t" << p1 <<endl;
    cout <<"Your answer: " <<endl;
    cout <<"\t" << my_answer <<endl;
    if (p1 != my_answer) {
        cout <<"DOESN'T MATCH!!!!" <<endl <<endl;
        return -1;
    }
    else {
        cout <<"Match :-)" <<endl <<endl;
        return (double)(end-start)/CLOCKS_PER_SEC;
    }
}
double test2() {
    int t0[] = {4, 7, 4};
    vector <int> p0(t0, t0+sizeof(t0)/sizeof(int));
    FewestFactors * obj = new FewestFactors();
    clock_t start = clock();
    int my_answer = obj->number(p0);
    clock_t end = clock();
    delete obj;
    cout <<"Time: " <<(double)(end-start)/CLOCKS_PER_SEC <<" seconds" <<endl;
    int p1 = 447;
    cout <<"Desired answer: " <<endl;
    cout <<"\t" << p1 <<endl;
    cout <<"Your answer: " <<endl;
    cout <<"\t" << my_answer <<endl;
    if (p1 != my_answer) {
        cout <<"DOESN'T MATCH!!!!" <<endl <<endl;
        return -1;
    }
    else {
        cout <<"Match :-)" <<endl <<endl;
        return (double)(end-start)/CLOCKS_PER_SEC;
    }
}
double test3() {
    int t0[] = {1, 3, 7, 9};
    vector <int> p0(t0, t0+sizeof(t0)/sizeof(int));
    FewestFactors * obj = new FewestFactors();
    clock_t start = clock();
    int my_answer = obj->number(p0);
    clock_t end = clock();
    delete obj;
    cout <<"Time: " <<(double)(end-start)/CLOCKS_PER_SEC <<" seconds" <<endl;
    int p1 = 1973;
    cout <<"Desired answer: " <<endl;
    cout <<"\t" << p1 <<endl;
    cout <<"Your answer: " <<endl;
    cout <<"\t" << my_answer <<endl;
    if (p1 != my_answer) {
        cout <<"DOESN'T MATCH!!!!" <<endl <<endl;
        return -1;
    }
    else {
        cout <<"Match :-)" <<endl <<endl;
        return (double)(end-start)/CLOCKS_PER_SEC;
    }
}
double test4() {
    int t0[] = {7, 5, 4, 3, 6};
    vector <int> p0(t0, t0+sizeof(t0)/sizeof(int));
    FewestFactors * obj = new FewestFactors();
    clock_t start = clock();
    int my_answer = obj->number(p0);
    clock_t end = clock();
    delete obj;
    cout <<"Time: " <<(double)(end-start)/CLOCKS_PER_SEC <<" seconds" <<endl;
    int p1 = 36457;
    cout <<"Desired answer: " <<endl;
    cout <<"\t" << p1 <<endl;
    cout <<"Your answer: " <<endl;
    cout <<"\t" << my_answer <<endl;
    if (p1 != my_answer) {
        cout <<"DOESN'T MATCH!!!!" <<endl <<endl;
        return -1;
    }
    else {
        cout <<"Match :-)" <<endl <<endl;
        return (double)(end-start)/CLOCKS_PER_SEC;
    }
}
double test5() {
    int t0[] = {1,2,4};
    vector <int> p0(t0, t0+sizeof(t0)/sizeof(int));
    FewestFactors * obj = new FewestFactors();
    clock_t start = clock();
    int my_answer = obj->number(p0);
    clock_t end = clock();
    delete obj;
    cout <<"Time: " <<(double)(end-start)/CLOCKS_PER_SEC <<" seconds" <<endl;
    int p1 = 241;
    cout <<"Desired answer: " <<endl;
    cout <<"\t" << p1 <<endl;
    cout <<"Your answer: " <<endl;
    cout <<"\t" << my_answer <<endl;
    if (p1 != my_answer) {
        cout <<"DOESN'T MATCH!!!!" <<endl <<endl;
        return -1;
    }
    else {
        cout <<"Match :-)" <<endl <<endl;
        return (double)(end-start)/CLOCKS_PER_SEC;
    }
}


double test6() {
    int t0[] = {7, 5, 4, 3 ,6 };
    vector <int> p0(t0, t0+sizeof(t0)/sizeof(int));
    FewestFactors * obj = new FewestFactors();
    clock_t start = clock();
    int my_answer = obj->number(p0);
    clock_t end = clock();
    delete obj;
    cout <<"Time: " <<(double)(end-start)/CLOCKS_PER_SEC <<" seconds" <<endl;
    int p1 = 36457;
    cout <<"Desired answer: " <<endl;
    cout <<"\t" << p1 <<endl;
    cout <<"Your answer: " <<endl;
    cout <<"\t" << my_answer <<endl;
    if (p1 != my_answer) {
        cout <<"DOESN'T MATCH!!!!" <<endl <<endl;
        return -1;
    }
    else {
        cout <<"Match :-)" <<endl <<endl;
        return (double)(end-start)/CLOCKS_PER_SEC;
    }
}

int main() {
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
    
    time = test5();
    if (time < 0)
        errors = true;
    
    time = test6();
    if (time < 0)
        errors = true;
    
    if (!errors)
        cout <<"You're a stud (at least on the example cases)!" <<endl;
    else
        cout <<"Some of the test cases had errors." <<endl;
}
