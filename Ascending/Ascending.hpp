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


//g++ -std=c++98 Ascending.hpp -o Ascending
//./Ascending

class Ascending{
public:
    int steps(vector<int> flights, int stairsPerStride){
        int s=0;
        for(int i=0;i<flights.size();i++){
            s+=flights[i]/stairsPerStride;
            if(flights[i]%stairsPerStride!=0){
                s+=1;
            }
        }
        s+=(flights.size()-1)*2;
        return s;
    }
};
