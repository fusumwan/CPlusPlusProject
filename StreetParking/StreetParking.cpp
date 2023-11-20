#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <climits>
#include <cfloat>
#include <map>
#include <utility>
#include <set>
#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include <list>
#include <algorithm>
#include <functional>
#include <sstream>
#include <complex>
#include <stack>
#include <queue>

//g++ StreetParking.cpp -o StreetParking
//./StreetParking




/*

Problem (3) StreetParking:
Initial thoughts:

In this program I am nesting two loops. If the string has n characters in the street name, 
the outer loop runs n times and the inner loop runs n times for each iteration of the outer loop, 
giving us n²​​ total comparsion with private driveway, bus stop, and side-street. 
Thus this function runs in O(n²) time. If the array has 10 characters, I have to compare strings more than 100 times. 


Pseudo-Code:


class StreetParking 
	public procedure freeParks (string street)
		parkinglist
		Repeat i<-0 until i<street.size()
			Driveway(parkinglist,i)
			BusStop(parkinglist,i)
			Street(parkinglist,i)
		EndRepeat
		total = 0;
		Repeat i<-0 until i<street.size()
			if(parkinglist[i] == 1)
				total+=1;
		return total; 
	
endclass


Reflection:

Example "---B--S-D--S--"

I would like to choose brute force. My freeParks functions have O(n) runtime, 
Sometimes the size of string is an actual number for each iteration of outer loop.
I can compare each position. So it works this way. Let make a example to explain.

First of all, creating a integer list for couting the parking place and the default value is 1 in each parking place. 1 mean parkingable and 0 is not avaliable for parking.
Checking the parking place at 0 meter(s) is not directly in front of a private driveway. 
 And then checking the parking place at 0 meter(s) is not directly in front of a bus stop. 
 And then checking the parking place at 0 meter(s) is not directly in front of a side-street. 
 Checking the parking place at 5 meter(s) is not directly in front of a private driveway. 
 And then checking the parking place at 5 meter(s) is not directly in front of a bus stop. 
 And then if the parking place at 15 meters before a bus stop and set zero. 
 And then checking the parking place at 5 meter(s) is not directly in front of a side-street. 
 Checking the parking place at 10 meter(s) is not directly in front of a private driveway. 
 And then checking the parking place at 10 meter(s) is not directly in front of a bus stop. 
 And then if the parking place at 15 meters before a bus stop and set zero. 
 And then checking the parking place at 10 meter(s) is not directly in front of a side-street. 
 Checking the parking place at 15 meter(s) is not directly in front of a private driveway. 
 And then checking the parking place at 15 meter(s) is not directly in front of a bus stop. 
 And then if the parking place at 15 meters before a bus stop and set zero. 
 And then checking the parking place at 15 meter(s) is not directly in front of a side-street. 
 Checking the parking place at 20 meter(s) is not directly in front of a private driveway. 
 And then checking the parking place at 20 meter(s) is not directly in front of a bus stop. 
 And then checking the parking place at 20 meter(s) is not directly in front of a side-street. 
 Checking the parking place at 25 meter(s) is not directly in front of a private driveway. 
 And then checking the parking place at 25 meter(s) is not directly in front of a bus stop. 
 And then checking the parking place at 25 meter(s) is not directly in front of a side-street. 
 And then if the parking place at 30 meters before a side-street and set zero. 
 Checking the parking place at 30 meter(s) is not directly in front of a private driveway. 
 And then checking the parking place at 30 meter(s) is not directly in front of a bus stop. 
 And then checking the parking place at 30 meter(s) is not directly in front of a side-street. 
 And then if the parking place at 30 meters before a side-street and set zero. 
 Checking the parking place at 35 meter(s) is not directly in front of a private driveway. 
 And then checking the parking place at 35 meter(s) is not directly in front of a bus stop. 
 And then checking the parking place at 35 meter(s) is not directly in front of a side-street. 
 And then if the parking place at 30 meters before a side-street and set zero. 
 Checking the parking place at 40 meter(s) is not directly in front of a private driveway. 
 If the parking place is at a private driveway then set zero. 
 And then checking the parking place at 40 meter(s) is not directly in front of a bus stop. 
 And then checking the parking place at 40 meter(s) is not directly in front of a side-street. 
 Checking the parking place at 45 meter(s) is not directly in front of a private driveway. 
 And then checking the parking place at 45 meter(s) is not directly in front of a bus stop. 
 And then checking the parking place at 45 meter(s) is not directly in front of a side-street. 
 Checking the parking place at 50 meter(s) is not directly in front of a private driveway. 
 And then checking the parking place at 50 meter(s) is not directly in front of a bus stop. 
 And then checking the parking place at 50 meter(s) is not directly in front of a side-street. 
 And then if the parking place at 55 meters before a side-street and set zero. 
 Checking the parking place at 55 meter(s) is not directly in front of a private driveway. 
 And then checking the parking place at 55 meter(s) is not directly in front of a bus stop. 
 And then checking the parking place at 55 meter(s) is not directly in front of a side-street. 
 And then if the parking place at 55 meters before a side-street and set zero. 
 Checking the parking place at 60 meter(s) is not directly in front of a private driveway. 
 And then checking the parking place at 60 meter(s) is not directly in front of a bus stop. 
 And then checking the parking place at 60 meter(s) is not directly in front of a side-street. 
 And then if the parking place at 55 meters before a side-street and set zero. 
 Checking the parking place at 65 meter(s) is not directly in front of a private driveway. 
 And then checking the parking place at 65 meter(s) is not directly in front of a bus stop. 
 And then checking the parking place at 65 meter(s) is not directly in front of a side-street. 
 Finally, the program have to count how much the parking place at street.
 So the total parking place is 4.

*/


using namespace std;

class StreetParking {
 public:
	string _street;
	vector<int> Driveway(vector<int> parkinglist,int i){
		if(_street[i] == 'D') {
				printf(" If the parking place is at a private driveway then set zero. \n");
				parkinglist[i] = 0;
			}
			return parkinglist;
	}
	vector<int> BusStop(vector<int> parkinglist,int i){
		for(int j=0;j<=2;++j){
				int p = i + j;
				if(p < 0 || p >= (int)_street.size()) {
					continue;
				}
				if(_street[p] == 'B') {
					printf(" And then if the parking place at %d meters before a bus stop and set zero. \n",5*p);
					parkinglist[i] = 0;
				}
			}
			return parkinglist;
	}
	vector<int> Street(vector<int> parkinglist,int i){
		for(int j=-1;j<=1;++j){
			int p = i + j;
			if(p < 0 || p >= (int)_street.size()) {
				continue;
			}
			if(_street[p] == 'S') {
				printf(" And then if the parking place at %d meters before a side-street and set zero. \n",5*p);
				parkinglist[i] = 0;
			}
		}
		return parkinglist;
	}
	int freeParks(string street) {
		_street=street;
		// -- main code --
		vector<int> parkinglist(street.size(), 1);
		for(int i=0;i<street.size();++i){
			printf(" Checking the parking place at %d meter(s) is not directly in front of a private driveway. \n",i*5);
			parkinglist=Driveway(parkinglist,i);
			
			printf(" And then checking the parking place at %d meter(s) is not directly in front of a bus stop. \n",i*5);
			
			parkinglist=BusStop(parkinglist,i);
			
			printf(" And then checking the parking place at %d meter(s) is not directly in front of a side-street. \n",i*5);
			parkinglist=Street(parkinglist,i);
			
		}
		printf(" Finally, the program have to count how much the parking place at street.\n");
		int total = 0;
		for(int i=0;i<street.size();i++){
			if(parkinglist[i] == 1){
				total+=1;
			}
		}
		printf(" So the total parking place is %d.\n",total);
		return total; 
	}

// BEGIN CUT HERE
/*
// PROBLEM STATEMENT
// You are looking for a place to park your car on a suburban street. You can park at any position that meets the following requirements:
1.  It is not directly in front of a private driveway.
2.  It is not directly in front of a bus stop.
3.  It is not 5 meters before a bus stop.
4.  It is not 10 meters before a bus stop.
5.  It is not directly in front of a side-street.
6.  It is not 5 meters before a side-street.
7.  It is not 5 meters after a side-street.
The street will be represented as a string, where each character describes a section of the street 5 meters in length. So the first character describes the first 5 meters of the street, the second character describes the next 5 meters and so on. street will use 'D' for driveway, 'B' for bus stop, 'S' for side-street and '-' for all other sections of the street. A position is directly in front of an object if it has the same index as the object in street. A position is before an object if its index is lower than the index of the object in street. Finally, a position is after an object if its index is higher than the index of the object in street.
Given the street return the total number of possible parking spaces on that street.
DEFINITION
Class:StreetParking
Method:freeParks
Parameters:string
Returns:int
Method signature:int freeParks(string street)
CONSTRAINTS
-street will have between 1 and 50 characters inclusive.
-street will only contain characters 'D', 'B', 'S' and '-'.
EXAMPLES
0)
"---B--S-D--S--"
Returns: 4
The street looks like this:
---B--S-D--S--
^   ^    ^   ^
|   |    |   |
The arrows indicate where you are allowed to park on this street. Thus the method should return 4.
1)
"DDBDDBDDBDD"
Returns: 0
This street is full of private driveways and bus stops. You cannot park anywhere on this street. The method should return 0.
2)
"--S--S--S--S--"
Returns: 2
You can only park at the first and last positions on this street. The method should return 2.
3)
"SSD-B---BD-DDSB-----S-S--------S-B----BSB-S--B-S-D"
Returns: 14
*/
// END CUT HERE

	
// BEGIN CUT HERE
	public:
	void run_test(int Case) { if ((Case == -1) || (Case == 0)) test_case_0(); if ((Case == -1) || (Case == 1)) test_case_1(); if ((Case == -1) || (Case == 2)) test_case_2(); if ((Case == -1) || (Case == 3)) test_case_3(); }
	private:
	template <typename T> string print_array(const vector<T> &V) { ostringstream os; os << "{ "; for (typename vector<T>::const_iterator iter = V.begin(); iter != V.end(); ++iter) os << '\"' << *iter << "\","; os << " }"; return os.str(); }
	void verify_case(int Case, const int &Expected, const int &Received) { cerr << "Test Case #" << Case << "..."; if (Expected == Received) cerr << "PASSED" << endl; else { cerr << "FAILED" << endl; cerr << "\tExpected: \"" << Expected << '\"' << endl; cerr << "\tReceived: \"" << Received << '\"' << endl; } }
	void test_case_0() { string Arg0 = "---B--S-D--S--"; int Arg1 = 4; verify_case(0, Arg1, freeParks(Arg0)); }
	void test_case_1() { string Arg0 = "DDBDDBDDBDD"; int Arg1 = 0; verify_case(1, Arg1, freeParks(Arg0)); }
	void test_case_2() { string Arg0 = "--S--S--S--S--"; int Arg1 = 2; verify_case(2, Arg1, freeParks(Arg0)); }
	void test_case_3() { string Arg0 = "SSD-B---BD-DDSB-----S-S--------S-B----BSB-S--B-S-D"; int Arg1 = 14; verify_case(3, Arg1, freeParks(Arg0)); }

// END CUT HERE

};

// BEGIN CUT HERE

int main()
{
	StreetParking test;
	test.run_test(-1);
	
	return 0;
}