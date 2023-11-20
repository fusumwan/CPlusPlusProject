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

//g++ -std=c++11 BigBurger.hpp -o BigBurger
//./BigBurger

class BigBurger {
    public:
    int ans;
    vector<int> finish;
    int maxWait(vector <int> arrival, vector <int> service) {
        int t = 0;
        int i=0;
        ans=0;
    	finish.clear();
        while(i < (int)arrival.size()){
            t = max(t, arrival[i]);
            finish.push_back(t);
            t += service[i];
            int diff= finish[i]-arrival[i];
            if(diff>ans)
                ans=diff;
            i+=1;
        }
        return ans;
    }
};
 
