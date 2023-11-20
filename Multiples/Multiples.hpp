#include <iostream>
#include <climits>
#include <map>
#include <vector>
#include <list>



//g++ -std=c++98 Multiples.hpp -o Multiples
//./Multiples

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
