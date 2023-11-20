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

//g++ -std=c++11 FewestFactors.cpp -o FewestFactors
//./FewestFactors
static string debug_message="";
template <typename T>
string to_string(T value)
{
    std::ostringstream os ;
    os << value ;
    return os.str() ;
}
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
            debug_message+="number("+to_string(number)+") % "+to_string(i)+":"+to_string(number % i)+"\n";
            debug_message+="value:"+to_string(value)+"\n";
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
        store.clear();
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


class FewestFactors {
public:
    int value = -1;
    int mf = 9999999;
    
    int number(vector<int> digits) {
        debug_message="";
        value = -1;
        mf = 9999999;
        if(digits.size()==3){
            debug_message+="search:digits[0]:"+to_string(digits[0])+":digits[1]:"+to_string(digits[1])+":digits[2]:"+to_string(digits[2])+"\n";
        }
        search(digits, 0);
        
        if(digits.size()==3){
            if(digits[0]==4 &&
               digits[1]==7 &&
               digits[2]==4){
                debug_message+="number.value:"+to_string(value)+"\n";
                //printf("%s\n",debug_message.c_str());
            
            }
        }
        
        return value;
    }
    int dn(vector<int> digits){
        int _value = 0;
        for(int i=0;i<digits.size();i++){
            _value = _value * 10 + digits[i];
            debug_message+="value:"+to_string(_value)+" * 10 + digits["+to_string(i)+"]:"+to_string(digits[i])+"\n";
        }
        debug_message+="dn.value:"+to_string(_value)+"\n";
        return _value;
    }
    
    bool isFactor(int number,int factorNum){
        if ((factorNum == mf && number < value) || factorNum < mf) { debug_message+="true:factorNum:"+to_string(factorNum)+":mf:"+to_string(mf)+":value:"+to_string(value)+":number:"+to_string(number)+"\n";
            return true;
            
        }
        debug_message+="isFactor.false:factorNum:"+to_string(factorNum)+":mf:"+to_string(mf)+":value:"+to_string(value)+":number:"+to_string(number)+"\n";
        return false;
    }
    
    void search(vector<int> digits, int index) {
        if (index == digits.size()) {
            int dg=dn(digits);
            int fn = 0;
            NodeFactor f;
            debug_message+="dg:"+to_string(dg)+"\n";
            f.number=dg;
            DP dp;
            Node n(1,dg,f,f,f);
            n.setI(1);
            n.BaseUpdate();
            fn=dp.Solution(1, n).value;
            debug_message+="fn:"+to_string(fn)+"\n";
            if (isFactor(dg,fn)) {
                value = dg;
                debug_message+="isFactor.value:"+to_string(value)+"\n";
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


