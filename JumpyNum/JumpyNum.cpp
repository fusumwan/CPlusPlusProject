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
/*
 Problem (5) JumpyNum:
 Initial thoughts:
 Since the problem mentioned is that the hop count is a positive integer, we must ensure that the hop count should be a positive number. And since all one digits are beating, there are 0 to 9 digits. Then, depending on the problem, the difference between all adjacent numbers is at least 2. Therefore, I have to use a recursive function to check the difference between all adjacent numbers.

 Pseudo-Code:
 

 class JumpyNum {
 public procedure howMany(int low, int high) {
     range.clear();
     int _high=numjmp(high);
     int _low=numjmp(low-1);
     return _high - _low;
 endclass
 
 Reflection:
 
 This problem is definitely using recursive function because you have to check the difference between all adjacent numbers. And then find out the number of jumpy. If I want to improve the performance of code,I would like to choose brute force.

 */



static string debug_message="";
typedef pair<int, int> pair_list;
map<pair_list, int> range;
template <typename T>
string to_string(T value)
{
    std::ostringstream os ;
    os << value ;
    return os.str() ;
}

int get_remain_bound(int bound, int last){
    int tmp=bound - last;
    tmp=tmp/10;
    return tmp;
}
extern int contain(int , int );

int count_jumpy(int bound, int last){
    int remain_bound=get_remain_bound(bound,last);
    int ans = (last != 0);
    int i=0;
    while(i <= 9){
        int tmp=last- i;
        tmp=abs(tmp);
        if (tmp >= 2) {
            ans += contain(remain_bound, i);
        }
        i+=1;
    }
    return ans;
}

int contain(int bound, int last) {
    int remain_bound=get_remain_bound(bound,last);
    if(0==bound)
    {
        return 0;
    }
    if (bound<last){
        return 0;
    }
    if (range.find(pair_list(bound, last)) != range.end()){
        return range[pair_list(bound, last)];
    }
    int ans=count_jumpy(bound,last);
    range[pair_list(bound, last)] = ans;
    return ans;
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
        if(i>number){
            value +=0;
        }
        else if(number==0){
            value +=0;
        }else{
            value += contain(number, i);
            debug_message+="i:"+to_string(i)+"F.value:"+to_string(value)+" number:"+to_string(number)+"\n";
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
        int def_len=999999999;
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
        //debug_message+="result.value:"+to_string(result.value)+"\n";
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


int numjmp(int num) {
    
    debug_message="";
    NodeFactor f;
    f.number=num;
    f.value=0;
    DP dp;
    Node n(0,9,f,f,f);
    n.setI(0);
    n.BaseUpdate();
    NodeFactor fn=dp.Solution(0, n);
    // printf("%s",debug_message.c_str());
    return fn.value;
}
class JumpyNum {
public:
    int howMany(int low, int high) {
        range.clear();
        int _high=numjmp(high);
        int _low=numjmp(low-1);
        return _high - _low;
    }
};

int main(){
    JumpyNum j;
    //cout<<j.howMany(1, 10)<<endl;
    //cout<<j.howMany(9, 23)<<endl;
    //cout<<j.howMany(2000000000, 2000000000)<<endl;
    cout<<j.howMany(8000, 20934)<<endl;
}
