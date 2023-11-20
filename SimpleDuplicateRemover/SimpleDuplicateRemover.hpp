#include <iostream>     // std::cout
#include <algorithm>    // std::reverse
#include <vector>       // std::vector
#include <climits>
#include <map>
#include <list>


using namespace std;

//g++ -std=c++98 SimpleDuplicateRemover.hpp -o SimpleDuplicateRemover
//./SimpleDuplicateRemover


class Node{
    int _i=0;
    int _min;
    int _max;
    vector <int> _factor;
    vector <int> _base;
    vector <int> _data;
public:
    Node(){
        _i=0;
        _min=0;
        _max=0;
        _factor;
        _base;
        _data;
    }
    Node(int min,int max,vector<int> factor,vector<int> base,vector <int> data){
        _min=min;
        _max=max;
        _factor=factor;
        _base=base;
        _data=data;
    }
    vector <int> get(){
        if(_i<_min || _i>_max)
            return _base;
        else if(_i<_factor.size()){
            if(!_base[_factor[_i]]){
                _base[_factor[_i]]=1;
                _data.push_back(_factor[_i]);
            }
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
    vector<int> getFactor(){
        return _factor;
    }
    void setI(int i){
        _i=i;
    }
    void BaseUpdate(){
        get();
    }
    vector<int> getBase(){
        return _base;
    }
    vector<int> getData(){
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
    vector<int> Solution(int i,Node n){
        vector<int> result;
        int len=n.getMax()-i;
        int def_len=1000;
        if(len<def_len){
            result= DecisionTree(i,n).getData();
        }else{
            int t=len/def_len;
            int r=len%def_len;
            int count=0;
            int turn=0;
            for(int x=i;x<n.getMax();x++){
                vector<int> base;
                vector<int> data;
                for(int i=0;i<10000;i++){
                    base.push_back(0);
                }
                Node nr(x,x,n.getFactor(),base,data);
                nr.setI(x);
                nr.BaseUpdate();
                store.clear();
                
                result.insert (result.begin(), DecisionTree(x,nr).getData().data(), DecisionTree(x,nr).getData().data()+3);
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

class SimpleDuplicateRemover{
public:
    vector <int> process(vector <int> s) {
        reverse(s.begin(), s.end());
        vector <int> base;
        vector <int> data;
        for(int i=0;i<10000;i++){
            base.push_back(0);
        }
        DP dp;
        Node n(0,(int)s.size(),s,base,data);
        n.BaseUpdate();
        base=dp.Solution(0, n);
        reverse(base.begin(), base.end());
        
        return base;
    }
};


