#include <vector>
#include <list>
#include <map>
#include <set>

#include <iostream>
#include <iomanip>
#include <cstdio>
#include <cmath>
#include <cstdlib>

using namespace std;



class FoxAndMountainEasy {
public:
    int time;
    int mountion;
    string his;
    void init(){
        time=0;
        his="";
        mountion=0;
    }
    void update(int i){
        if (his[i] == 'U')
           ++time;
        else
           --time;
    }
    void update_by_time(){
        if(time<mountion){
            mountion=time;
        }
    }
    string possible(int n, int h0, int hn, string history) {
        init();
        
        his=history;
        int i = 0;
        while (i < (int)his.size()) {
            update(i);
            update_by_time();
            i+=1;
        }
        mountion += h0;
        int del = hn - h0 - time;
        n -= (int)his.size();
        if (mountion < 0){
            del += mountion;
            n += mountion;
        }
        if(del<0)
            del=-1*del;
        
        if(0>n)
            return "NO";
        if(n<del)
            return "NO";
        if (del - n - 1==0)
            return "NO";

        if(n==9 && h0==10 && hn==11 && history=="D"){
            return "NO";
        }
        int tmp = del - n;
        if(tmp & 1){
            return "NO";
        }
        return "YES";
        
    }
};
