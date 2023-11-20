// BEGIN CUT HERE
// END CUT HERE
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


//g++ -std=c++98 DerivativeSequence.cpp -o DerivativeSequence
//./DerivativeSequence

/*
Problem (1) DerivativeSequence:
Initial thoughts:

This question given a series of K elements, we can calculate the difference sequence by taking the difference 
between each pair of adjacent elements. So in this program I am nesting two loops. 
If the sequence has s number in the vector, the outer loop runs n times of derivative sequence of order and the inner loop runs n times for each iteration of the outer loop, 
giving us s*(n​-1) total comparsion with the difference between each pair of adjacent elements. 
Thus this function runs in O(s*(n-1) time. If the array has 10 integers, I have to compare strings more than 18 times. 

Pseudo-Code:

class DerivativeSequence
	public procedure  UpdateSeq(vector <int> result,int i){
				vector <int> next;
				int j=0;
				int j_size=result.size()-1;
				Repeat (j<=0,j_size)
					next.push_back(result[j+1] - result[j]);
				EndRepeat
				result.clear();
				result = next;
				return result;
	endprocedure
	public procedure  derSeq(vector <int> a, int n)
			vector <int> result;
			result = a;
			int i=0;
			Repeat (i<=0,n){
				result=UpdateSeq(result,i);
			EndRepeat
			return result;
	endprocedure
endclass

Reflection:
I would like to choose brute force. My UpdateSeq functions have O(s*(n-1)) runtime, 
Sometimes the size of string is an actual number for each iteration of outer loop.
I can find the difference between each pair of adjacent elements. 
So it works this way. Let make a example to explain.

If I want to improve solution I should have practical understanding of the Algorithms 
(e.g. When to Use Divide and Conquer, Why double hashing required?, 
Where brute force concept can be applied?) 

Example:
For instance, the difference sequence of {5,6,3,9,-1} is {6-5,3-6,9-3,-1-9} = {1,-3,6,-10}. 
Formally, the difference sequence of the sequence a1, a2, ... , ak is b1, b2, ... , bk-1, 
where bi = ai+1 - ai.
Starting to the number 1 of derivative sequence. 
Finding the difference between the pair of adjacent elements 6-5 . 
Finding the difference between the pair of adjacent elements 3-6 . 
Finding the difference between the pair of adjacent elements 9-3 . 
Finding the difference between the pair of adjacent elements -1-9 . 

*/

typedef int int_type;
#define FOR(x, a, b) for(int_type x = static_cast<int_type>(a); x < static_cast<int_type>(b); ++x)



class DerivativeSequence {
public:
	vector <int> UpdateSeq(vector <int> result,int i){
				vector <int> next;
				int j=0;
				int j_size=result.size()-1;
				FOR(j,0,j_size){
					printf("Finding the difference between the pair of adjacent elements %d-%d . \n",result[j+1],result[j]);
					next.push_back(result[j+1] - result[j]);
				}
				result.clear();
				result = next;
				return result;
	}
	vector <int> derSeq(vector <int> a, int n)
	{
			vector <int> result;
			result = a;
			int i=0;
			FOR(i,0,n){
				printf("Starting to the number %d of derivative sequence. \n",n);
					
				result=UpdateSeq(result,i);
			}
			return result;
	}
	 
	// BEGIN CUT HERE
public:
	void run_test(int Case) { if ((Case == -1) || (Case == 0)) test_case_0(); if ((Case == -1) || (Case == 1)) test_case_1(); if ((Case == -1) || (Case == 2)) test_case_2(); if ((Case == -1) || (Case == 3)) test_case_3(); if ((Case == -1) || (Case == 4)) test_case_4(); }
private:
	template <typename T> string print_array(const vector<T> &V) { ostringstream os; os << "{ "; for (typename vector<T>::const_iterator iter = V.begin(); iter != V.end(); ++iter) os << '\"' << *iter << "\","; os << " }"; return os.str(); }
	void verify_case(int Case, const vector <int> &Expected, const vector <int> &Received) { cerr << "Test Case #" << Case << "..."; if (Expected == Received) cerr << "PASSED" << endl; else { cerr << "FAILED" << endl; cerr << "\tExpected: " << print_array(Expected) << endl; cerr << "\tReceived: " << print_array(Received) << endl; } }
	void test_case_0() { int Arr0[] = {5,6,3,9,-1}; vector <int> Arg0(Arr0, Arr0 + (sizeof(Arr0) / sizeof(Arr0[0]))); int Arg1 = 1; int Arr2[] = {1, -3, 6, -10 }; vector <int> Arg2(Arr2, Arr2 + (sizeof(Arr2) / sizeof(Arr2[0]))); verify_case(0, Arg2, derSeq(Arg0, Arg1)); }
	void test_case_1() { int Arr0[] = {5,6,3,9,-1}; vector <int> Arg0(Arr0, Arr0 + (sizeof(Arr0) / sizeof(Arr0[0]))); int Arg1 = 2; int Arr2[] = {-4, 9, -16 }; vector <int> Arg2(Arr2, Arr2 + (sizeof(Arr2) / sizeof(Arr2[0]))); verify_case(1, Arg2, derSeq(Arg0, Arg1)); }
	void test_case_2() { int Arr0[] = {5,6,3,9,-1}; vector <int> Arg0(Arr0, Arr0 + (sizeof(Arr0) / sizeof(Arr0[0]))); int Arg1 = 4; int Arr2[] = {-38 }; vector <int> Arg2(Arr2, Arr2 + (sizeof(Arr2) / sizeof(Arr2[0]))); verify_case(2, Arg2, derSeq(Arg0, Arg1)); }
	void test_case_3() { int Arr0[] = {4,4,4,4,4,4,4,4}; vector <int> Arg0(Arr0, Arr0 + (sizeof(Arr0) / sizeof(Arr0[0]))); int Arg1 = 3; int Arr2[] = {0, 0, 0, 0, 0 }; vector <int> Arg2(Arr2, Arr2 + (sizeof(Arr2) / sizeof(Arr2[0]))); verify_case(3, Arg2, derSeq(Arg0, Arg1)); }
	void test_case_4() { int Arr0[] = {-100,100}; vector <int> Arg0(Arr0, Arr0 + (sizeof(Arr0) / sizeof(Arr0[0]))); int Arg1 = 0; int Arr2[] = {-100, 100 }; vector <int> Arg2(Arr2, Arr2 + (sizeof(Arr2) / sizeof(Arr2[0]))); verify_case(4, Arg2, derSeq(Arg0, Arg1)); }

	// END CUT HERE

};
// BEGIN CUT HERE
int main() {
DerivativeSequence ___test;
___test.run_test(-1);
}
// END CUT HERE 