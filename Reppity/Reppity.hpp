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

using namespace std;

//g++ Reppity.hpp -o Reppity
//./Reppity

class NodeFactor{
public:
    int i;
    string input;
    int ie;
    int value;
    NodeFactor(){
        i=0;
        input="";
        ie=0;
        value=0;
    }
    int findpos(int i,int j){
        string findstr=input.substr(i, j - i + 1);
        int len=j+1;
        int position=0;
        position=input.find(findstr, len);
        return position;
    }

    void F(int i){
        for (int j = i; j < ie; ++j) {
            value = (string::npos!=findpos(i,j))? max(value, j - i + 1):value;
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



class Reppity {
public:
    int longestRep(string input) {
        int mx = 0;
        NodeFactor f;
        f.input=input;
        f.ie=(int)input.size();
        DP dp;
        Node n(0,(int)input.size(),f,f,f);
        n.BaseUpdate();
        NodeFactor base=dp.Solution(1, n);
        return base.value;
    }
};
