#include <iostream>
#include <string>
#include <vector>

//g++ -std=c++98 TimeTravellingCellar.hpp -o TimeTravellingCellar
//./TimeTravellingCellar

using namespace std;

class TimeTravellingCellar {
public:
   int determineProfit(vector<int> profit, vector<int> decay ){
		int ans = 0;
		for(int i=0;i<(int)profit.size();i++){
			for(int j=0;j<(int)decay.size();j++){
				if(i!=j&&(int)profit[i]-decay[j]>ans){
					ans = profit[i]-decay[j];
				}
			}
		}
		return ans;
	}
};
