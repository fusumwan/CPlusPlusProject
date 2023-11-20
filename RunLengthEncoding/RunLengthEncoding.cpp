#include <iostream>
#include <algorithm>
#include <vector>
#include <climits>
#include <map>
#include <list>
#include <string>
#include <vector>


#include <set>
#include <deque>
#include <stack>
#include <bitset>

#include <functional>
#include <numeric>
#include <utility>
#include <sstream>

#include <iomanip>
#include <cstdio>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <iterator>
#include <tuple>
#include <regex>
#include <array>
#include <valarray>
#define all(v)begin(v),end(v)
#define dump(v)copy(all(v), ostream_iterator<decltype(*m.begin())>(cout, "\n"))
#define rg(i,a,b)for(int i=a,i##e=b;i<i##e;++i)
#define fr(i,n)for(int i=n,i##e=0;i<i##e;++i)
#define rf(i,n)for(int i=n-1;i>=0;--i)
#define ei(a,m)for(auto&a:m)if(int a##i=&a-&*begin(m)+1)if(--a##i,1)
#define sz(v)int(v.size())
#define sr(v)sort(all(v))
#define rs(v)sort(all(v),greater<int>())
#define rev(v)reverse(all(v))
#define eb emplace_back
#define big numeric_limits<int>::max()
#define g(t,i)get<i>(t)
#define cb(v,w)copy(all(v),back_inserter(w))
#define uni(v)sort(all(v));v.erase(unique(all(v)),end(v))
#define vt(...)vector<tuple<__VA_ARGS__>>
#define smx(a,b)a=max(a,b)
#define smn(a,b)a=min(a,b)
typedef long long ll;
using namespace std;

//g++ -std=c++98 RunLengthEncoding.cpp -o RunLengthEncoding
//./RunLengthEncoding



/*

Problem (3) RunLengthEncoding:
Initial thoughts:
Due to the question request a decode coding function which is a simple compression technique that compresses a string of letters by 
replacing repeated consecutive letters  with the number of occurrences of the letter, 
followed by the letter. So I you the dynamic programming to decode the string.


Pseudo-Code:

class RunLengthEncoding {
    public procedure decode(string text)
        int i=0;
        int length = 0;
        string ret;
        string data="";
        DP dp;
        Node n(0,(int)text.size(),text,0,data);
        n.setI(0);
        n.BaseUpdate();
        ret=dp.Solution(n.getMin(), n);
        if(ret=="" && text!="")
            ret="TOO LONG";
        
        return ret;
    endprocedure
endclass

Reflection:

Actually, when I was counting the string, 

I got the index wrong, so sometime the program will return empty result.

So I try to set the to "TOO LONG" when the input/output is empty or length of string larger than 50.


*/


class Node{
    int _i=0;
    int _min;
    int _max;
    string _factor;
    int _base=0;
    string _data;
    
public:
    Node(){
        _i=0;
        _min=0;
        _max=0;
        _factor;
        _base;
        _data;
    }
    Node(int min,int max,string factor,int base,string data){
        _min=min;
        _max=max;
        _factor=factor;
        _base=base;
        _data=data;
    }
    
    int p_count(){
        int p = 0;
        while (_factor[_i] >= '0' &&
               _factor[_i] <= '9'){
            int p1=p * 10;
            int p2=(_factor[_i] - '0');
            p = p1 +p2;
            _i++;
        }
        return p;
    }
    bool check(){
        if (_base > 50){
            _data="TOO LONG";
            return false;
        }
        return true;
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
    string getFactor(){
        return _factor;
    }
    void setI(int i){
        _i=i;
    }
    int getI(){
        return _i;
    }
    void BaseUpdate(){
        get();
    }
    int setBase(string base){
        return _base;
    }
    int getBase(){
        return _base;
    }
    string getData(){
        return _data;
    }
    bool end(){
        if(_i>_max)
            return true;
        return false;
    }
    string get(){
        if(_i<_min || _i>_max)
            return _data;
        else if(_i<_factor.size()){
            if (_factor[_i] >= '0' && _factor[_i] <= '9'){
                int p = p_count();
                _base += p;
                if (!check()){ return _data; }
                for (int j=0; j<p; j++)
                    _data.push_back(_factor[_i]);
            }
            else
            {
                _data.push_back(_factor[_i]);
                _base++;
            }
            if (!check()){ return _data; }
        }
        return _data;
    }
};

class DP{
public:
    
    std::map<int, Node> store;
    string Solution(int i,Node n){
        string result;
        int len=n.getMax()-i;
        int def_len=3000;
        if(len<def_len){
            result= DecisionTree(i,n).getData();
        }else{
            int t=len/def_len;
            int r=len%def_len;
            int count=0;
            int turn=0;
            for(int x=i;x<n.getMax();x++){
                int base=0;
                string data="";
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
        if (n.end() || n.getData()=="TOO LONG"){
            return n;
        }
        else if (store.find(i)!= store.end()) {
            return (Node) store[i];
        }else{
            Node next(n.getMin(),n.getMax(),n.getFactor(),n.getBase(),n.getData());
            next.setI(n.getI()+1);
            next.BaseUpdate();
            Node result = DecisionTree(n.getI()+1,next);
            store[i]=result;
            return result;
        }
    }
};


class RunLengthEncoding {
public:
    string decode(string text)
    {
        int i=0;
        int length = 0;
        string ret;
        string data="";
        DP dp;
        Node n(0,(int)text.size(),text,0,data);
        n.setI(0);
        n.BaseUpdate();
        ret=dp.Solution(n.getMin(), n);
        if(ret=="" && text!="")
            ret="TOO LONG";
        
        return ret;
    }
};


// BEGIN KAWIGIEDIT TESTING
// Generated by KawigiEdit-pf 2.3.0
#include <iostream>
#include <string>
#include <vector>
#include <ctime>
#include <cmath>
using namespace std;
bool KawigiEdit_RunTest(int testNum, string p0, bool hasAnswer, string p1) {
    cout << "Test " << testNum << ": [" << "\"" << p0 << "\"";
    cout << "]" << endl;
    RunLengthEncoding *obj;
    string answer;
    obj = new RunLengthEncoding();
    clock_t startTime = clock();
    answer = obj->decode(p0);
    clock_t endTime = clock();
    delete obj;
    bool res;
    res = true;
    cout << "Time: " << double(endTime - startTime) / CLOCKS_PER_SEC << " seconds" << endl;
    if (hasAnswer) {
        cout << "Desired answer:" << endl;
        cout << "\t" << "\"" << p1 << "\"" << endl;
    }
    cout << "Your answer:" << endl;
    cout << "\t" << "\"" << answer << "\"" << endl;
    if (hasAnswer) {
        res = answer == p1;
    }
    if (!res) {
        cout << "DOESN'T MATCH!!!!" << endl;
    } else if (double(endTime - startTime) / CLOCKS_PER_SEC >= 2) {
        cout << "FAIL the timeout" << endl;
        res = false;
    } else if (hasAnswer) {
        cout << "Match :-)" << endl;
    } else {
        cout << "OK, but is it right?" << endl;
    }
    cout << "" << endl;
    return res;
}
int main() {
    bool all_right;
    bool disabled;
    bool tests_disabled;
    all_right = true;
    tests_disabled = false;
    
    string p0;
    string p1;
    
    // ----- test 0 -----
    disabled = false;
    p0 = "4A3BC2DE";
    p1 = "AAAABBBCDDE";
    all_right = (disabled || KawigiEdit_RunTest(0, p0, true, p1) ) && all_right;
    tests_disabled = tests_disabled || disabled;
    // ------------------
    
    // ----- test 1 -----
    disabled = false;
    p0 = "1A1B1C1D1E";
    p1 = "ABCDE";
    all_right = (disabled || KawigiEdit_RunTest(1, p0, true, p1) ) && all_right;
    tests_disabled = tests_disabled || disabled;
    // ------------------
    
    // ----- test 2 -----
    disabled = false;
    p0 = "1A3A5A4BCCCC";
    p1 = "AAAAAAAAABBBBCCCC";
    all_right = (disabled || KawigiEdit_RunTest(2, p0, true, p1) ) && all_right;
    tests_disabled = tests_disabled || disabled;
    // ------------------
    
    // ----- test 3 -----
    disabled = false;
    p0 = "50A";
    p1 = "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA";
    all_right = (disabled || KawigiEdit_RunTest(3, p0, true, p1) ) && all_right;
    tests_disabled = tests_disabled || disabled;
    // ------------------
    
    // ----- test 4 -----
    disabled = false;
    p0 = "21Z13S9A8M";
    p1 = "TOO LONG";
    all_right = (disabled || KawigiEdit_RunTest(4, p0, true, p1) ) && all_right;
    tests_disabled = tests_disabled || disabled;
    // ------------------
    
    // ----- test 5 -----
    disabled = false;
    p0 = "123456789012345678901234567890B";
    p1 = "TOO LONG";
    all_right = (disabled || KawigiEdit_RunTest(5, p0, true, p1) ) && all_right;
    tests_disabled = tests_disabled || disabled;
    // ------------------
    
    if (all_right) {
        if (tests_disabled) {
            cout << "You're a stud (but some test cases were disabled)!" << endl;
        } else {
            cout << "You're a stud (at least on given cases)!" << endl;
        }
    } else {
        cout << "Some of the test cases had errors." << endl;
    }
    return 0;
}
// END KAWIGIEDIT TESTING
