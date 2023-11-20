#include <algorithm>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <queue>
#include <stack>
#include <set>
#include <map>
#include <cstdio>
#include <cstdlib>
#include <cctype>
#include <climits>
#include <cmath>

using namespace std;

//g++ BlackAndRed.hpp -o BlackAndRed
//./BlackAndRed

class NodeFactor{
public:
    int i;
    string deck;
    int p;
    int mp;
    int value;
    NodeFactor(){
        i=0;
        deck="";
        p=0;
        mp=0;
        value=0;
    }
    bool lessthan(){
        return (p < mp)? true :false;
    }
    void F(int i){
        update(i);
        if (lessthan()) {
            mp = p;
            value = i + 1;
        }
    }
    void update(int i){
        if (deck[i]=='R') {
            p-=1;
        } else {
            p+=1;
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
        int def_len=1;
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

class BlackAndRed {
public:
    int cut(string deck) {
        int res = 0;
        int minpoint = 0;
        int point = 0;
        
        NodeFactor f;
        f.deck=deck;
        DP dp;
        Node n(0,deck.size(),f,f,f);
        n.BaseUpdate();
        NodeFactor base=dp.Solution(1, n);
        return base.value;
    }
};
