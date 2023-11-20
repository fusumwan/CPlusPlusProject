#include <iostream>
#include <algorithm>
#include <vector>
#include <climits>
#include <map>
#include <list>
#include <cmath>
#include <string>

using namespace std;

//g++ -std=c++98 NumberSplit.cpp -o NumberSplit
//./NumberSplit


/*

Problem (4) NumberSplit:
Initial thoughts:
As the question mention that The decimal representation of a given number is first divided into several (at least two) parts, 
which are then multiplied to obtain a possible successor. 

For the selected successor, 
we repeat this process to get the third number, and so on, 
until we reach a single digit.
So it just use a recursive function to cater this situation.


Reflection:
Sometimes, it will get error, because it cann't change number to binary in correct result.


*/

class NumberSplit{
public:
	vector<int> gen;
	vector<int> numlist;
	
	void initalGen(){
		gen.push_back(1);
		gen.push_back(0);
		gen.push_back(0);
		gen.push_back(1);
	}
	int recursive(int num, int base1 ,int base2) {
		if (num < base1) { return 1; }
		if (numlist[num] > base2) { return numlist[num]; }
		numlist[num] = 0;
		for (int i = 1; i < toDecimal(num); i++) { numlist[num] = max(numlist[num], recursive(create(num, i),base1,base2));
		}
		return numlist[num]+1;
	}
	
	void splitout(int i){
		if (((i >> gen[2]) & 1) == 1) {
			gen[0] *= gen[1];
			gen[3]  = 1;
			gen[1]  = 0;
		}
	}
	void inital(){
		for(int i=0;i<10000;i++){
			numlist.push_back(-1);
		}
	}
	int create(int n, int split) {
		for(int i=n;i>0;i/=10){
			int module_r = i % 10;
			gen[1] += module_r * gen[3];
			gen[3] *= 10;
			splitout(split);
			gen[2]++;
		}
		if (gen[1] > 0) {
			gen[0] *= gen[1];
		}
		
		return gen[0];
	}
	int toDecimal(int n){
		int i = 0;
		while (n > 0) {

			n = n / 10;
			i++;
		}
		return 1 << (i-1);
	}
	int longestSequence(int start) {
		inital();
		initalGen();
		return recursive(start,10,-1);
	}
};




int main(){
	NumberSplit ns;
	cout<<ns.longestSequence(6)<<endl;
	int len=1 << 2;
	cout<<len<<endl;
}
