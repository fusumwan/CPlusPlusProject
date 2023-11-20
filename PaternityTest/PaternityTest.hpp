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

using namespace std;
//g++ PaternityTest.hpp -o PaternityTest
//./PaternityTest



class NodeFactor{
public:
    
    vector<int> value;
    string child;
    string mother;
    vector <string> men;
    NodeFactor(){
        child="";
        mother="";
    }
    void F(int i){
        
        int len=child.length();
        int possible = 1;
        
        //printf("i:%d\n",i);
        int count = 0;
        possible_find(i,&possible,&count,len);
        
        update_finded(i,&possible,count,len);
        //printf("ok:%d   ans.size():%d\n",ok, (int)ans.size());
    }
    void possible_find(int i,int *possible,int *count,int len){
        int j=0;
        while(j < len)
        {
            int find = 0;
            find+=((child[j] == mother[j])? 1 : 0);
            find+=((child[j] == men[i][j])? 2 : 0);
            
            if (find == 0){
                (*possible) = 0;
            }
            else if (2 <= find){
                (*count)+=1;
            }
            j+=1;
        }
    }
    void update_finded(int i, int *possible, int count, int len){
        if ((*possible) && count + count < len) (*possible) = 0;
        if ((*possible)) value.push_back(i);
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
        int def_len=9999999;
        if(len<def_len){
            result= DecisionTree(i,n).getData();
        }else{
            int turn=0;
            for(int x=i;x<n.getMax()-1;x++){
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

class PaternityTest{
public:
    vector <int> possibleFathers(string child, string mother, vector <string> men) {
        vector<int> ans;
        printf("=====start=====\n");
        NodeFactor f;
        f.child=child;
        f.mother=mother;
        f.men=men;
        DP dp;
        Node n(0,(int)men.size()-1,f,f,f);
        n.setI(0);
        n.BaseUpdate();
        NodeFactor base=dp.Solution(0,n);
        return base.value;
    }
};



