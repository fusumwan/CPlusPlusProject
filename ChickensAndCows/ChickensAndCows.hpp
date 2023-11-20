#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <utility>
#include <set>
#include <cctype>
#include <queue>
#include <stack>
using namespace std;

//g++ -std=c++98 ChickensAndCows.cpp -o ChickensAndCows
//./ChickensAndCows


class ChickensAndCows{
public:
    vector<int> howMany(int heads, int legs){
        int tmp=legs;
        int cows=0;
        int chickens=0;
        int h=heads;
        vector<int> num;
        if(heads*4==legs){
            num.push_back(0);
            num.push_back(heads);
            return num;
        }
        else if(heads*2>legs){
            //num.push_back(0);
            //num.push_back(0);
            return num;
        }
        else if(heads*4>legs){
            while(tmp-2>=0 && h-1>=0){
                tmp=tmp-2;
                chickens+=1;
                h-=1;
                if(h*4==tmp){
                    cows=h;
                    num.push_back(chickens);
                    num.push_back(cows);
                }
            }
        }
        

        if(tmp==0){
            
            num.push_back(chickens);
            num.push_back(cows);
        }
        return num;
    }
};

