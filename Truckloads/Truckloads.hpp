#include <iostream>
#include <string>

using namespace std;
//g++ -std=c++98 Truckloads.hpp -o Truckloads
//./Truckloads


class Truckloads {
public:
	int numTrucks(int numCrates, int loadSize) {
		if (numCrates <= loadSize) return 1;
		int branchLeft=0;
		int branchRight=0;
		if(numCrates%2==0){
			branchLeft=numCrates/2;
			branchRight=numCrates/2;
		}else{
			branchLeft=(numCrates+1)/2;
			branchRight=(numCrates-1)/2;
		}
		return numTrucks(branchLeft, loadSize) + numTrucks(branchRight, loadSize);
	}
};