#include <iostream>
#include <string>
#include <vector>
#include <stack>
//g++ -std=c++98 RugSizes.hpp -o RugSizes
//./RugSizes

using namespace std;

class RugSizes {
public:
	int rugCount(int area) {
		int ans = 0;
		int i = 1;
		while(i*i<=area){
			if (area % i != 0) {
				i++;
				continue;
			}
			int s = area / i;
			if (i != s && i % 2 == 0 && s % 2 == 0) {
				i++;
				continue;
			}
			ans++;
			i++;
		}
		return ans;
	}
};


