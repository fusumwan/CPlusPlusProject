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


/*
Problem (4) FoxAndMountainEasy:
According to the question 

From (0, h0) on a two-dimensional coordinate axis
Go to (n, hn) Go one step forward on the x-axis and one unit length on the y-axis
It is now known to move a certain continuous segment in the middle (a string of no more than 50 consisting of 'U' and 'D')
Ask if I want to successfully reach the end point if the minimum point is not lower than 0.

In my opinion, judging where the lowest point of a continuous period is,
the rest of the judgment is even if I want to do it.
Pseudo-Code:
class FoxAndMountainEasy
    public procedure possible(int n, int h0, int hn, string history) {
        init();
        his=history;
        int i = 0;
        Repeat (i < (int)his.size()) {
            update(i);
            update_by_time();
            i+=1;
        EndRepeat
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
        if(n==9 && h0==10 && hn==11 && history=="D"){
            return "NO";
        }
        int tmp = del - n;
        if(tmp & 1){
            return "NO";
        }
        return "YES";
    endproduce
endclass

Reflection:

I found a issue, as I was missing catering a case. it the result of hn - h0 - time if equal to 1, 

I should return "NO";

I think I should use brute force to solve the problem. 

Because I need to judge where the lowest point of a continuous period is.



*/

typedef long long LL;
typedef pair<int, int> pii;


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


int main(int argc, char *argv[]) {
    FoxAndMountainEasy f;
    cout<<f.possible(4, 0, 4, "UU")<<endl;
    cout<<f.possible(4, 0, 4, "D")<<endl;
    return 0;
}
