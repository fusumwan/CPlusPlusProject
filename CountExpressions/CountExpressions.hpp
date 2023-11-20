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
using namespace std;

template <typename T>
string to_string(T value)
{
    std::ostringstream os ;
    os << value ;
    return os.str() ;
}

static string debug_message="";
class NodeFactor{
public:
    
    vector<int> poss;
    int value;
    int base;
    int x;
    int y;
    int cx;
    int cy;
    int key;
    NodeFactor(){
        value=0;
        base=0;
        x=0;
        y=0;
        cx=0;
        cy=0;
        key=0;
    }
    void F(int i){
        if(poss[i]==base)value++;
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
        store.clear();
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
        debug_message+="result.value:"+to_string(result.value)+"\n";
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



class CountExpressions
{
public:
    vector<int> expression;
    int calcExpressions(int x, int y, int val)
    {
        expression.clear();
        NodeFactor opf1,opf2;
        opf1.x=opf2.x=opf1.key=x;
        opf1.y=opf2.y=opf2.key=y;
        
        opf1.cx=opf2.cy=1;
        opf1.cy=opf2.cx=0;
        operators(opf1);
        operators(opf2);
    
        int result = 0;
        
        NodeFactor f;
        f.poss=expression;
        f.base=val;
        DP dp;
        Node n(0,(int)expression.size(),f,f,f);
        n.setI(0);
        n.BaseUpdate();
        result=dp.Solution(1, n).value;
        return result;
    }
    
    void express_update(NodeFactor opf){
        if(opf.cx==2 &&
           opf.cy==2)
        {
            expression.push_back(opf.key);
        }
    }
    NodeFactor getMode(NodeFactor opf,int i){
        switch (i) {
            case 1:
                opf.key+=opf.x;
                opf.cx+=1;
                break;
            case 2:
                opf.key-=opf.x;
                opf.cx+=1;
                break;
            case 3:
                opf.key*=opf.x;
                opf.cx+=1;
                break;
            case 4:
                opf.key+=opf.y;
                opf.cy+=1;
                break;
            case 5:
                opf.key-=opf.y;
                opf.cy+=1;
                break;
            case 6:
                opf.key*=opf.y;
                opf.cy+=1;
                break;
            default:
                break;
        }
        return opf;
    }
    void operators(NodeFactor opf)
    {
        express_update(opf);
        if(opf.cx<2)
        {
            operators(getMode(opf,1));
            operators(getMode(opf,2));
            operators(getMode(opf,3));
        }
        if(opf.cy<2)
        {
            operators(getMode(opf,4));
            operators(getMode(opf,5));
            operators(getMode(opf,6));
        }
    }
};


