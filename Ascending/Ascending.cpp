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

//g++ -std=c++98 Ascending.cpp -o Ascending
//./Ascending
//g++ -std=c++98 Ascending.cpp -o Ascending
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


int main()
{
    Ascending A;
    cout<<A.steps({15},2)<<endl;
    cout<<A.steps({15,15},2)<<endl;
    cout<<A.steps({5,11,9,13,8,30,14},3)<<endl;
    return 0;
}

